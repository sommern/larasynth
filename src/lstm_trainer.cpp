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

#include "lstm_trainer.hpp"

using namespace std;
using namespace larasynth;

LstmTrainer::LstmTrainer(  LstmNetwork& untrained_network,
                             TrainingEventStream& training_stream,
                             TrainingConfig& training_config,
                             LstmConfig& network_config,
                             MidiTranslator& midi_translator,
                             size_t update_period )
  : _network( untrained_network )
  , _training_stream( training_stream )
  , _training_config( training_config )
  , _network_config( network_config )
  , _midi_translator( midi_translator )
  , _update_period( update_period )
{
  _network.zero_network();
}

void LstmTrainer::feed_forward_next( ctrl_values_t& target_ctrl_values,
                                      ctrl_values_t& output_ctrl_values,
                                      feedback_source source,
                                      bool print ) {
  vector<double> input = _midi_translator.get_input( source );
  _network.feed_forward( input );

  vector<double> output = _network.get_output();
  _midi_translator.report_output( output );

  if( print ) {
    vector<double> target = _midi_translator.get_target();

    ctrl_values_t output_values;
    vector<double> hot_output;

    _midi_translator.ctrl_vals_and_hot_output( output, output_values,
                                               hot_output );

    cout << "Input: ";
    for( auto val : input )
      cout << val << " ";
    cout << endl;

    cout << "Target:  ";
    for( auto val : target )
      cout << val << " ";
    cout << endl;

    cout << "Output:  ";
    for( auto val : hot_output )
      cout << val << " ";
    cout << endl << endl;
  }

  target_ctrl_values = _midi_translator.get_target_ctrl_values();
  output_ctrl_values = _midi_translator.get_output_ctrl_values();  
}

double LstmTrainer::calculate_error( const ctrl_values_t& target_ctrl_values,
                                      const ctrl_values_t& output_ctrl_values )
{
  double sse = 0.0;

  for( auto& kv : output_ctrl_values ) {
    event_data_t ctrl = kv.first;
    event_data_t output_value = kv.second;
    event_data_t target_value = target_ctrl_values.at( ctrl );

    double error = output_value - target_value;

    sse += error * error;
  }

  return sse;
}

void LstmTrainer::advance_stream_until_update_time() {
  bool time_to_update = false;

  while( !time_to_update ) {
    size_t next_time = _training_stream.get_next_time();
    event_data_t next_type = _training_stream.get_next_type();

    // no new events to register
    if( next_time > _next_update_time ) {
      _current_time = _next_update_time;
      _next_update_time += _update_period;

      time_to_update = true;
    }
    // next event is a note event and should be presented immediately
    else if( next_type == NOTE_ON || next_type == NOTE_OFF ) {
      Event event = _training_stream.get_next();

      _midi_translator.report_note_event( &event );

      _current_time = event.time();
      _next_update_time = _current_time + _update_period;

      time_to_update = true;
    }
    // next event is a control event which occurs before the next update
    // time. only present if this is the last event
    else {
      Event event = _training_stream.get_next();

      assert( event.type() == CTRL_CHANGE );

      _midi_translator.update_ctrl_value( event.controller(), event.value() );

      if( !_training_stream.has_next() )
        time_to_update = true;
    }
  }
}

void LstmTrainer::run_training_epoch() {
  ++_epoch;

  if( _training_config.get_zero_network_before_each_epoch() )
    _network.zero_network();

  _current_time = 0;
  _next_update_time = 0;

  size_t consecutive_failure_count = 0;
  size_t streak = 0;
  _new_best_streak = false;

  bool reset = false;

  _training_stream.reset( _training_config.get_example_repetitions() );
  _midi_translator.reset();

  while( _training_stream.has_next() && !reset ) {
    advance_stream_until_update_time();

    ctrl_values_t target_ctrl_values;
    ctrl_values_t output_ctrl_values;      

    feed_forward_next( target_ctrl_values, output_ctrl_values,
                       TARGET_SOURCE );

    double sse = calculate_error( target_ctrl_values, output_ctrl_values );

    bool correct = ( sse == 0 );

    if( correct ) {
      ++streak;
      consecutive_failure_count = 0;

      if( streak > _max_streak ) {
        _max_streak = streak;
        _new_best_streak = true;
      }
    }
    else {
      streak = 0;

      if( sse > _training_config.get_squared_error_failure_tolerance() )
        ++consecutive_failure_count;
      else
        consecutive_failure_count = 0;
    }

    if( should_backpropogate( correct ) ) {
      _network.backpropagate( _midi_translator.get_target(),
                              _network_config.get_learning_rate(),
                              _network_config.get_momentum() );
    }
    if( should_reset( correct, consecutive_failure_count ) ) {
      reset = true;
      if( prob_bool( _training_config.get_zero_network_on_reset() ) )
        _network.zero_network();
    }
  }
}


bool LstmTrainer::should_reset( bool correct,
                                 size_t consecutive_failure_count ) {
  size_t failure_count_for_reset =
    _training_config.get_consecutive_failures_for_reset();

  if( correct || consecutive_failure_count < failure_count_for_reset )
    return false;
  else
    return prob_bool( _training_config.get_reset_probability() );
}


bool LstmTrainer::should_backpropogate( bool correct ) {
  if( !correct )
    return true;

  bool backpropogate_if_correct =
    prob_bool( _training_config.get_backpropagate_if_correct() );

  if( backpropogate_if_correct )
    return true;

  return false;
}


bool LstmTrainer::should_validate() {
  return ( _epoch % _training_config.get_epoch_count_before_validating() == 0 ||
           _new_best_streak );
}

LstmResult LstmTrainer::validate() {
  if( prob_bool( _training_config.get_zero_network_before_validation() ) ||
      _training_config.get_zero_network_before_each_epoch() )
    _network.zero_network();

  LstmResult result( _epoch );

  size_t feed_forward_count = 0;
  double sse = 0.0;

  size_t incorrectly_classified_count = 0;

  _current_time = 0;
  _next_update_time = 0;

  _training_stream.reset( _training_config.get_validation_example_repetitions() );
  _midi_translator.reset();
    
  while( _training_stream.has_next() ) {
    advance_stream_until_update_time();

    feed_forward_count += 1;

    ctrl_values_t target_ctrl_values;
    ctrl_values_t output_ctrl_values;

    feed_forward_next( target_ctrl_values, output_ctrl_values,
                       OUTPUT_SOURCE );

    double squared_error = calculate_error( target_ctrl_values,
                                            output_ctrl_values );

    if( squared_error != 0.0 )
      ++incorrectly_classified_count;

    sse += squared_error;

    result.add_target( target_ctrl_values );
    result.add_output( output_ctrl_values );
    result.add_cell_states( _network.get_cell_states() );
  }

  double mse = sse / (double)feed_forward_count;

  result.set_mse( mse );

  return result;
}

bool LstmTrainer::prob_bool( double probability ) {
  if( probability == 1.0 )
    return true;

  return probability >= _rand_gen.uniform_real( 0.0, 1.0 );
}
