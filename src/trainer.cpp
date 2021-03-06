/*
Copyright 2016 Nathan Sommer

This file is part of Larasynth.

Larasynth is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Larasynth is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Larasynth.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "trainer.hpp"

using namespace std;
using namespace larasynth;
using namespace littlelstm;

bool Trainer::_keep_running = true;

Trainer::Trainer( const string& config_directory_path,
                  volatile sig_atomic_t* shutdown_flag )
  : _shutdown_flag( shutdown_flag )
{
  ConfigDirectory dir( config_directory_path );
  dir.process_directory();

  if( !dir.training_examples_exist() ) {
    string error = "There are no training examples in the directory " +
      config_directory_path + ".\nCould not train.";
    throw TrainerException( error );
  }

  TrainingResults results( dir.get_new_training_results_filename(),
                           WRITE_RESULTS );

  ConfigParser cp( dir.get_config_file_path() );
  ConfigParameters lstm_params = cp.get_section_params( "lstm" );
  ConfigParameters seq_params = cp.get_section_params( "representation" );
  ConfigParameters midi_params = cp.get_section_params( "midi" );
  ConfigParameters training_params = cp.get_section_params( "training" );

  TrainingConfig training_config( training_params );

  MidiConfig midi_config( midi_params );   

  RepresentationConfig repr_config( seq_params );

  vector<string> example_filenames = dir.get_training_example_filenames();

  size_t update_period =
    MICROSECONDS_PER_SECOND / repr_config.get_update_rate();

  TrainingEventStream training_stream( update_period,
                                training_config.get_tempo_adjustment_factor(),
                                training_config.get_tempo_jitter_factor(),
                                training_config.get_mean_padding(),
                                training_config.get_padding_stddev(),
                                midi_config.get_ctrl_defaults() );

  training_stream.add_examples( example_filenames );

  MidiTranslator translator( repr_config.get_ctrl_output_counts(),
                             repr_config.get_input_feature_config(),
                             midi_config.get_ctrl_defaults(),
                             training_stream.get_min_max(),
                             TRAIN );

  LstmConfig lstm_config( lstm_params );

  lstm_config.set_input_count( translator.get_input_count() );
  lstm_config.set_output_count( translator.get_output_count() );

  littlelstm::LstmArchitecture arch( lstm_config.get_input_count(),
                                     lstm_config.get_output_count(),
                                     lstm_config.get_block_counts() );

  littlelstm::LstmNetwork net( arch );
  
  LstmTrainer trainer( net, training_stream, training_config, lstm_config,
                        translator, update_period );

  double best_mse = INFINITY;
  LstmResult best_result;
  WeightsMap_t best_weights;

  cout << "Training LSTM network." << endl << endl;
  cout << "Network configuration:" << endl << endl;
  lstm_config.print_network_configuration();
  cout << endl;

  cout << "Training configuration:" << endl << endl;
  training_config.print_training_configuration();
  cout << endl;

  cout << "Representation configuration:" << endl << endl;
  repr_config.print_representation_configuration();
  cout << endl;

  size_t max_epoch_count;

  if( training_config.get_max_epoch_count() > 0 )
    max_epoch_count = training_config.get_max_epoch_count();
  else
    max_epoch_count = numeric_limits<size_t>::max();

  Timer training_timer;
  Timer since_last_report_timer;

  while( trainer.get_epoch() <= max_epoch_count && !*_shutdown_flag ) {
    trainer.run_training_epoch();

    if( since_last_report_timer.get_elapsed_minutes() >= 1.0 ) {
      since_last_report_timer.start();
      double elapsed_minutes = training_timer.get_elapsed_minutes();
      
      cout << endl;
      cout << (int)elapsed_minutes << " minute(s) elapsed" << endl;
      cout << trainer.get_epoch() / elapsed_minutes << " epochs per minute"
           << endl;
      if( best_mse != INFINITY )
        cout << "Best MSE: " << best_mse << " after epoch " <<
             best_result.get_epoch() << endl;
      cout << endl;
    }

    if( trainer.should_validate() ) {
      cout << "Validating after epoch " << trainer.get_epoch() << endl;

      LstmResult result = trainer.validate();

      cout << "MSE: " << result.get_mse() << endl;

      if( result.get_mse() < best_mse ) {
        best_mse = result.get_mse();
        best_weights = net.get_weights_map();
        best_result = result;
        cout << "New best MSE" << endl;
      }

      if( result.get_mse() <= training_config.get_mse_threshold() ) {
        cout << "MSE threshold hit after " << trainer.get_epoch() << " epochs"
             << endl;
        break;
      }
    }
  }

  net.set_weights( best_weights );

  results.add_network( net );
  results.add_lstm_config( lstm_config );
  results.add_repr_config( repr_config );
  results.add_training_config( training_config );
  results.add_min_max( training_stream.get_min_max() );

  results.add_result( best_result );

  cout << endl << "Writing results to " << results.get_filename() << endl;

  results.write();
}
