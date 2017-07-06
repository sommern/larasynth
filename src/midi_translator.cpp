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

#include "midi_translator.hpp"

using namespace std;
using namespace larasynth;

MidiTranslator::MidiTranslator( const unordered_map<event_data_t,size_t>&
                                ctrl_output_counts,
                                feature_config_t input_feature_config,
                                const ctrl_values_t& ctrl_defaults,
                                const MidiMinMax& min_max,
                                run_mode mode )
  : _min_max( min_max )
  , _input_feature_config( input_feature_config )
  , _mode( mode )
  , _ctrl_output_counts( ctrl_output_counts )
  , _ctrl_defaults( ctrl_defaults )
  , _note_velocities( NOTE_MAX - NOTE_MIN + 1, 0 )
  , _previous_note_velocities( _note_velocities )
{
  vector<event_data_t> ctrls;
  ctrls.reserve( _ctrl_output_counts.size() );

  for( auto& kv : _ctrl_output_counts )
    ctrls.push_back( kv.first );

  sort( ctrls.begin(), ctrls.end() );

  size_t begin_i = 0;

  for( event_data_t ctrl : ctrls ) {
    _ctrl_output_begin_is[ctrl] = begin_i;
    begin_i += _ctrl_output_counts[ctrl];

    setup_ctrl_maps( ctrl );
  }

  adjust_ctrl_values( _ctrl_defaults );
  _output_ctrl_values = _ctrl_defaults;
  _target_ctrl_values = _ctrl_defaults;  

  _output_count = 0;

  for( auto& kv : _ctrl_output_counts )
    _output_count += kv.second;

  _input_feature_count = 0;

  for( input_feature_t feature : input_feature_list ) {
    if( _input_feature_config[feature] ) {
      _input_feature_count += input_feature_sizes.at( feature );
    }
  }

  // FIXME: toggle feed back output
  _input_count = _input_feature_count + _output_count;

  _previous_output = vector<double>( _output_count, 0.0 );
  
  // filling the previous output as a target here will give it the default
  // controller values
  fill_target( _previous_output );
  _previous_target = _previous_output;
}

void MidiTranslator::adjust_ctrl_values( ctrl_values_t& ctrl_values ) {
  for( auto& kv : ctrl_values ) {
    event_data_t ctrl = kv.first;
    event_data_t ctrl_value = kv.second;

    size_t hot_index = _ctrl_value_to_hot_index[ctrl][ctrl_value];
    event_data_t adjusted_value = _output_to_ctrl_value[ctrl][hot_index];

    ctrl_values[ctrl] = adjusted_value;
  }
}
  
void MidiTranslator::report_note_event( const Event* note_event ) {
  assert( note_event->type() == NOTE_ON || note_event->type() == NOTE_OFF );

  event_data_t type;

  if( note_event->type() == NOTE_OFF || note_event->velocity() == 0 )
    type = NOTE_OFF;
  else
    type = NOTE_ON;

  event_data_t velocity;

  if( type == NOTE_ON )
    velocity = note_event->velocity();
  else
    velocity = 0;

  event_data_t pitch = note_event->pitch();

  // ignore duplicate NOTE_OFF events
  if( type == NOTE_OFF && _note_velocities[pitch] == 0 )
    return;

  _last_event_type = type;
  _note_velocities[pitch] = velocity;
  _last_velocity = velocity;

  if( _input_feature_config[INTERVAL] && type == NOTE_ON ) {
    if( _last_pitch > NOTE_MAX )
      _last_pitch = pitch;

    _last_interval = pitch - _last_pitch;
    _last_pitch = pitch;
  }
}

void MidiTranslator::update_ctrl_values( const ctrl_values_t& new_values ) {
  _target_ctrl_values = new_values;
  adjust_ctrl_values( _target_ctrl_values );
}

void MidiTranslator::update_ctrl_value( event_data_t ctrl,
                                        event_data_t value ) {
  if( _target_ctrl_values[ctrl] != value ) {
    _target_ctrl_values[ctrl] = value;
    adjust_ctrl_values( _target_ctrl_values );
  }
}

void MidiTranslator::setup_ctrl_maps( event_data_t ctrl ) {
  event_data_t min = _min_max.get_ctrl_min( ctrl );
  event_data_t max = _min_max.get_ctrl_max( ctrl );

  size_t output_count = _ctrl_output_counts[ctrl];

  vector<double> spaced_values = linspace( min, max, output_count );

  _output_to_ctrl_value[ctrl] = vector<event_data_t>();

  for( double value : spaced_values )
    _output_to_ctrl_value[ctrl].push_back( lround( value ) );

  size_t min_hot_index = 0;
  size_t max_hot_index = output_count - 1;

  vector<size_t> indexes;
  indexes.reserve( 128 );

  for( event_data_t ctrl_value = 0; ctrl_value < 128; ++ctrl_value ) {
    size_t i;

    // out of range values map to min or max value
    if( ctrl_value <= min ) {
      i = min_hot_index;
    }
    else if( ctrl_value >= max ) {
      i = max_hot_index;
    }
    else {
      i = index_of_closest_value( spaced_values, ctrl_value );
    }

    indexes.push_back( i );
  }

  _ctrl_value_to_hot_index[ctrl] = indexes;
}


void MidiTranslator::reset() {
  _target_ctrl_values = _ctrl_defaults;
  _output_ctrl_values = _ctrl_defaults;
  fill( _previous_output.begin(), _previous_output.end(), 0.0 );
  fill_target( _previous_output );
  _note_velocities = vector<event_data_t>( 128, 0 );
}


size_t MidiTranslator::index_of_closest_value( vector<double> spaced_values,
                                               event_data_t ctrl_value ) {
  auto upper_it = upper_bound( spaced_values.begin(), spaced_values.end(),
                               ctrl_value );

  size_t upper_i = upper_it - spaced_values.begin();

  double lower_value = *(upper_it - 1);
  double upper_value = *upper_it;

  double lower_difference = ctrl_value - lower_value;
  double upper_difference = upper_value - ctrl_value;

  if( lower_difference < upper_difference )
    return upper_i - 1;
  else
    return upper_i;
}


/**
 * Build a vector of count doubles starting with min and ending with
 * approximately max. The values in between are approximately evenly spaced
 * out.
 *
 * min must be less than max.
 */
vector<double> MidiTranslator::linspace( double min, double max,
                                         size_t count ) {
  double stride = ( max - min ) / ( count - 1 );

  vector<double> spaced_values;
  spaced_values.reserve( count );

  double value = min;

  // the final value might not be quite equal to max, but it'll be close
  // enough for our purposes
  for( size_t i = 0; i < count; ++i ) {
    spaced_values.push_back( value );
    value += stride;
  }

  return spaced_values;
}

vector<double> MidiTranslator::get_input( feedback_source source ) {
  vector<double> input( _input_count, 0.0 );
  fill_input( input, source );
  return input;
}

vector<double> MidiTranslator::get_target() {
  vector<double> target( _output_count, 0.0 );
  fill_target( target );
  return target;
}

void MidiTranslator::fill_input( vector<double>& input,
                                 feedback_source source ) {
  fill( input.begin(), input.end(), 0.0 );

  auto max_velocity_it = max_element( _note_velocities.begin(),
                                      _note_velocities.end() );

  size_t input_i = 0;

  if( _input_feature_config[SOME_NOTE_ON] ) {
    if( *max_velocity_it > 0 ) {
      input[input_i] = 1.0;
    }    
    ++input_i;
  }

  if( _input_feature_config[NOTE_STRUCK] ) {
    if( _last_event_type == NOTE_ON )
      input[input_i] = 1.0;
    ++input_i;
  }

  if( _input_feature_config[NOTE_RELEASED] ) {
    if( _last_event_type == NOTE_OFF )
      input[input_i] = 1.0;
    ++input_i;
  }

  if( _input_feature_config[VELOCITY] ) {
    input[input_i] = _last_velocity / VEL_MAX;
    ++input_i;
  }

  if( _input_feature_config[INTERVAL] ) {
    if( _last_event_type == NOTE_ON ) {
      size_t distance = abs( _last_interval );
      input[input_i + distance % 12] = 1.0;
      input_i += 12;

      if( _last_interval > 0 )
        input[input_i] = 1.0;

      ++input_i;

      if( _last_interval < 0 )
        input[input_i] = 1.0;

      ++input_i;

      if( distance >= 12 )
        input[input_i] = 1.0;

      ++input_i;
    }
    else
      input_i += input_feature_sizes.at( INTERVAL );
  }

  _last_event_type = NO_EVENT;
  _last_interval = 0;

  // feed back previous output with the input
  if( source == OUTPUT_SOURCE )
    copy( _previous_output.begin(), _previous_output.end(),
          input.begin() + _input_feature_count );
  else if( source == TARGET_SOURCE )
    copy( _previous_target.begin(), _previous_target.end(),
          input.begin() + _input_feature_count );
}

void MidiTranslator::fill_target( vector<double>& target ) {
  fill( target.begin(), target.end(), 0.0 );

  for( auto& kv : _target_ctrl_values ) {
    event_data_t ctrl = kv.first;
    event_data_t ctrl_value = kv.second;

    size_t hot_index = _ctrl_output_begin_is[ctrl] +
      _ctrl_value_to_hot_index[ctrl][ctrl_value];

    target[hot_index] = 1.0;
  }
}

void MidiTranslator::report_output( const vector<double>& output ) {
  _previous_output = vector<double>( output.size(), 0.0 );
  ctrl_vals_and_hot_output( output, _output_ctrl_values, _previous_output );

  if( _mode == TRAIN )
    fill_target( _previous_target );
}

void
MidiTranslator::ctrl_vals_and_hot_output( const vector<double>& output,
                                          unordered_map<event_data_t,
                                          event_data_t>& ctrl_values,
                                          vector<double>& hot_output ) {
  hot_output.resize( output.size() );
  fill( hot_output.begin(), hot_output.end(), 0.0 );

  for( auto& kv : _ctrl_output_counts ) {
    event_data_t ctrl = kv.first;
    size_t count = kv.second;

    size_t begin_i = _ctrl_output_begin_is[ctrl];
    size_t end_i = begin_i + ( count - 1);

    auto max_it = max_element( output.begin() + begin_i,
                               output.begin() + end_i + 1 );

    size_t argmax = distance( output.begin() + begin_i, max_it );

    ctrl_values[ctrl] = _output_to_ctrl_value[ctrl][argmax];

    hot_output[argmax + begin_i] = 1.0;
  }
}
