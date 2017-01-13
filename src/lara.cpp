/**
 * Main entry point for larasynth.
 *
 * Parses the command line arguments and initiates the appropriate actions.
 * 
 */

#include <iostream>
#include <string>
#include <signal.h>

#include "config_directory.hpp"
#include "interactive_prompt.hpp"
#include "config_parser.hpp"
#include "config_parameters.hpp"
#include "midi_config.hpp"
#include "rtmidi_client.hpp"
#include "representation_config.hpp"
#include "recorder.hpp"
#include "trainer.hpp"
#include "performer.hpp"

using namespace std;
using namespace larasynth;

volatile sig_atomic_t lara_shutdown_flag = 0;

void signal_handler( int signal ) {
  lara_shutdown_flag = 1;
}

void print_usage_and_exit( int argc, char** argv ) {
  cerr << "Usage: " << argv[0]
       << " <action> <project directory>" << endl << endl
       << "Actions:" << endl << endl
       << "  config  - create the project directory if it does not exist and create " << endl
       << "            a larasynth.conf file in the project directory" << endl
       << "  record  - record an example MIDI performance" << endl
       << "  train   - train a model using the training example(s)" << endl
       << "  perform - use one of the trained models to control a synthesizer's continuous" << endl
       << "            controllers during performance" << endl;
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
 * Train a model.
 */
void train( const string& directory_name ) {
  Trainer( directory_name, &lara_shutdown_flag );
}

void perform( const string& directory_name ) {
  ConfigDirectory dir( directory_name );
  dir.process_directory();

  ConfigParser cp( dir.get_config_file_path() );

  ConfigParameters lstm_params = cp.get_section_params( "lstm" );
  ConfigParameters seq_params = cp.get_section_params( "representation" );
  ConfigParameters midi_params = cp.get_section_params( "midi" );

  MidiConfig midi_config( midi_params );

  vector<string> results_filenames = dir.get_training_results_filenames();

  vector<TrainingResults> training_results;

  vector<ip_choice_t> choices;

  int choice_number = 1;
  for( auto& filename : results_filenames ) {
    training_results.emplace_back( filename, READ_RESULTS );

    ostringstream description;
    description << "MSE: " << training_results.back().get_mse() << " - "
                << filename;

    choices.emplace_back( to_string( choice_number ), description.str() );
    ++choice_number;
  }
  
  choices.emplace_back( "q", "quit" );

  size_t choice_i = pick_choice( "Pick from the following training results:",
                                 choices );

  if( choice_i >= results_filenames.size() )
    exit( EXIT_FAILURE );

  TrainingResults results( results_filenames[choice_i], READ_RESULTS );

  MidiMinMax min_max = results.get_min_max();

  LstmNetwork net = results.get_trained_network();

  RepresentationConfig repr_config = results.get_repr_config();

  RtMidiClient midi_client( "larasynth",
                            midi_config.get_performing_source_port(),
                            midi_config.get_performing_destination_port(),
                            PERFORM );

  Performer p( &midi_client, net, midi_config, repr_config, min_max,
               &lara_shutdown_flag );
}

/**
 * larasynth entry point.
 */
int main( int argc, char** argv ) {
  // we need an action and a project directory
  if( argc != 3 )
    print_usage_and_exit( argc, argv );

  signal( SIGINT, signal_handler );

  string action = argv[1];
  string directory_name = argv[2];

  try {
    if( action == "config" )
      config( directory_name );
    else if( action == "record" )
      record( directory_name );
    else if( action == "train" )
      train( directory_name );
    else if( action == "perform" )
      perform( directory_name );
    else {
      cerr << action << " is not a valid action." << endl << endl;
      print_usage_and_exit( argc, argv );
    }
  }
  catch( runtime_error& e ) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }

  exit( EXIT_SUCCESS );
}

