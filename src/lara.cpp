/** @file
 * Main entry point for larasynth.
 *
 * Parses the command line arguments and initiates the appropriate actions.
 * 
 */

#include <iostream>
#include <string>
#include <signal.h>
#include <map>

#include "config_directory.hpp"
#include "interactive_prompt.hpp"
#include "config_parser.hpp"
#include "config_parameters.hpp"
#include "midi_config.hpp"
#include "rtmidi_client.hpp"
#include "representation_config.hpp"
#include "performing_config.hpp"
#include "recorder.hpp"
#include "trainer.hpp"
#include "performer.hpp"
#include "midi_file_reader.hpp"

using namespace std;
using namespace larasynth;

volatile sig_atomic_t lara_shutdown_flag = 0;

void signal_handler( int signal ) {
  lara_shutdown_flag = 1;
}

void print_usage_and_exit( int argc, char** argv ) {
  cerr << "Usage: " << argv[0]
       << " <project directory> <action>" << endl << endl
       << "Actions:" << endl << endl
       << " config                 - create the project directory if it does not exist and" << endl
       << "                          create a larasynth.conf file in the project directory" << endl
       << " record                 - record a training example via a MIDI port" << endl
       << " import <MIDI filename> - create a training example from a MIDI file" << endl
       << " train                  - train a model using the current training example(s)" << endl
       << " perform [-v]           - use one of the trained models to control a" << endl
       << "                          synthesizer's continuous controllers during" << endl
       << "                          performance" << endl;
  exit( EXIT_FAILURE );
}

/**
 * Create the project directory if it does not exist and create an example
 * configuration file within it.
 */
void config( const string& directory_name ) {
  ConfigDirectory config_dir( directory_name );

  if( !config_dir.exists() ) {
    cout << directory_name << " does not exist." << endl;
    string question = "Create it now?";

    if( pick_yes_or_no( question, 'y' ) ) {
      config_dir.create();
      cout << "Created " << directory_name << endl;
    }
    else {
      cout << "Exiting." << endl;
      exit( EXIT_FAILURE );
    }
  }
      
  bool write_file = true;

  if( config_dir.config_file_exists() ) {
    cout << config_dir.get_config_file_path() << " already exists." << endl;
    string question = "Overwrite this file?";

    write_file = pick_yes_or_no( question, 'n' );
  }

  if( write_file ) {
    config_dir.write_example_config_file();
    cout << "Wrote " << config_dir.get_config_file_path() << endl;
    cout << "Edit this file to finish setting up the configuration." << endl;
  }
  else {
    cout << "Not writing file." << endl;
    exit( EXIT_FAILURE );
  }
}
  
/**
 * Record a training example.
 */
void record( const string& directory_name ) {
  ConfigDirectory dir( directory_name );
  dir.process_directory();

  ConfigParser cp( dir.get_config_file_path() );
  ConfigParameters params = cp.get_section_params( "midi" );
  MidiConfig mc( params );

  RtMidiClient midi_client( "larasynth", mc.get_recording_source_port(), "",
                            RECORD );

  Recorder r( dir.get_new_training_example_filename(), mc.get_ctrls(),
              &midi_client, &lara_shutdown_flag );
  r.record();
}

/**
 * Import a training example from a MIDI file.
 */
void import( const string& directory_name, const string& midi_filename ) {
  ConfigDirectory dir( directory_name );
  dir.process_directory();

  ConfigParser cp( dir.get_config_file_path() );
  ConfigParameters params = cp.get_section_params( "midi" );
  MidiConfig mc( params );

  MidiFileReader reader( midi_filename, mc.get_ctrls() );
  vector<Event> events = reader.get_events();

  cout << "Read events from " << reader.get_track_count() << " tracks in "
       << midi_filename << ": " << endl
       << "  " << reader.get_note_on_count() << " notes" << endl
       << "  " << reader.get_ctrl_change_count() << " controller events"
       << endl;

  string training_example_filename = dir.get_new_training_example_filename();

  write_events( events, training_example_filename );

  cout << "Wrote " << events.size() << " events to "
       << training_example_filename << endl;
}

/**
 * Train a model.
 */
void train( const string& directory_name ) {
  Trainer( directory_name, &lara_shutdown_flag );
}

/**
 * Perform using a trained model.
 */
void perform( const string& directory_name, bool verbose ) {
  ConfigDirectory dir( directory_name );
  dir.process_directory();

  ConfigParser cp( dir.get_config_file_path() );

  ConfigParameters lstm_params = cp.get_section_params( "lstm" );
  ConfigParameters seq_params = cp.get_section_params( "representation" );
  ConfigParameters midi_params = cp.get_section_params( "midi" );
  ConfigParameters perform_params = cp.get_section_params( "performing" );

  PerformingConfig perform_config( perform_params );

  MidiConfig midi_config( midi_params );

  vector<string> results_filenames = dir.get_training_results_filenames();

  map<string, string> filenames_by_display_filename;

  for( string& filename : results_filenames ) {
      size_t last_slash_i = filename.find_last_of( "/" );

      string display_filename;

      if( last_slash_i != string::npos ) {
        display_filename = filename.substr( last_slash_i + 1 );
      }
      else {
        display_filename = filename;
      }

      filenames_by_display_filename[display_filename] = filename;
  }

  string results_filename = perform_config.get_training_results_filename();

  if( results_filename != "" ) {
    if( filenames_by_display_filename.count( results_filename ) == 0 ) {
      cout << "Configuration error: " << results_filename << " does not exist."
           << endl;
      exit( EXIT_FAILURE );
    }

    results_filename = filenames_by_display_filename[results_filename];
  }
  else {
    vector<TrainingResults> training_results;

    vector<ip_choice_t> choices;

    int choice_number = 1;
    for( auto& kv : filenames_by_display_filename ) {
      string display_filename = kv.first;
      string filename = kv.second;

      training_results.emplace_back( filename, READ_RESULTS );

      ostringstream description;
      description << "MSE: " << training_results.back().get_mse() << " - "
                  << display_filename;

      choices.emplace_back( to_string( choice_number ), description.str() );
      ++choice_number;
    }
  
    choices.emplace_back( "q", "quit" );

    size_t choice_i = pick_choice( "Pick from the following training results:",
                                   choices );

    if( choice_i >= results_filenames.size() )
      exit( EXIT_FAILURE );

    results_filename = results_filenames[choice_i];

    cout << endl;
  }

  try {
    TrainingResults results( results_filename, READ_RESULTS );

    MidiMinMax min_max = results.get_min_max();

    LstmNetwork net = results.get_trained_network();

    RepresentationConfig repr_config = results.get_repr_config();

    RtMidiClient midi_client( "larasynth",
                              midi_config.get_performing_source_port(),
                              midi_config.get_performing_destination_port(),
                              PERFORM );

    Performer p( &midi_client, net, midi_config, repr_config, min_max,
                 &lara_shutdown_flag, verbose );
  }
  catch( const TrainingResultsException& e ) {
    cerr << "Error reading " << results_filename << endl
         << "Please choose a different file or re-train" << endl;
  }
}

/**
 * larasynth entry point.
 */
int main( int argc, char** argv ) {
  // we need a project directory and an action
  if( argc < 3 )
    print_usage_and_exit( argc, argv );

  signal( SIGINT, signal_handler );

  string directory_name = argv[1];
  string action = argv[2];

  map< string, vector<int> > action_argc = {
    { "config", { 3 } },
    { "record", { 3 } },
    { "import", { 4 } },
    { "train", { 3 } },
    { "perform", { 3, 4 } }
  };

  if( action_argc.count( action ) == 0 ) {
    cerr << action << " is not a valid action." << endl << endl;
    print_usage_and_exit( argc, argv );
  }

  if( find( action_argc[action].begin(),
            action_argc[action].end(),
            argc ) == action_argc[action].end() ) {
    cerr << "Invalid parameters for " << action << endl << endl;
    print_usage_and_exit( argc, argv );
  }

  try {
    if( action == "config" ) {
      config( directory_name );
    }
    else if( action == "record" ) {
      record( directory_name );
    }
    else if( action == "import" ) {
      string midi_filename = argv[3];
      import( directory_name, midi_filename );
    }
    else if( action == "train" ) {
      train( directory_name );
    }
    else if( action == "perform" ) {
      bool verbose = false;

      if( argc == 4 ) {
        if( strcmp( argv[3], "-v" ) == 0 ) {
          verbose = true;
        }
        else {
          cerr << "Unknown argument " << argv[3] << endl;
          print_usage_and_exit( argc, argv );
        }
      }

      perform( directory_name, verbose );
    }
  }
  catch( runtime_error& e ) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }

  exit( EXIT_SUCCESS );
}

