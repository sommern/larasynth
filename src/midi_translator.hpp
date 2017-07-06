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

#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <numeric>

#include "debug.hpp"
#include "representation_config.hpp"
#include "midi_config.hpp"
#include "midi_min_max.hpp"
#include "representation_config.hpp"
#include "run_modes.hpp"

namespace larasynth {

enum feedback_source { OUTPUT_SOURCE, TARGET_SOURCE, NO_FEEDBACK_SOURCE };

class MidiTranslator {
public:
  MidiTranslator( const std::unordered_map<event_data_t,size_t>&
                  ctrl_output_counts,
                  feature_config_t input_feature_config,
                  const ctrl_values_t& ctrl_defaults,
                  const MidiMinMax& min_max, run_mode mode );

  size_t get_input_count() { return _input_count; }
  size_t get_output_count() { return _output_count; }

  std::vector<double> get_input( feedback_source source );
  std::vector<double> get_target();

  void fill_input( std::vector<double>& input, feedback_source source );
  void fill_target( std::vector<double>& target );

  ctrl_values_t get_target_ctrl_values() const
  { return _target_ctrl_values; }
  ctrl_values_t get_output_ctrl_values() const
  { return _output_ctrl_values; }

  void update_ctrl_values( const ctrl_values_t& new_values );
  void update_ctrl_value( event_data_t ctrl, event_data_t value );
  void report_note_event( const Event* note_event );
  void report_output( const std::vector<double>& output );

  void reset();

  void ctrl_vals_and_hot_output( const std::vector<double>& output,
                                 std::unordered_map<event_data_t,
                                 event_data_t>& ctrl_values,
                                 std::vector<double>& hot_output );

private:
  void adjust_ctrl_values( ctrl_values_t& ctrl_values );

  void setup_ctrl_maps( event_data_t ctrl );
  size_t index_of_closest_value( std::vector<double> spaced_values,
                                 event_data_t ctrl_value );
  std::vector<double> linspace( double min, double max, size_t count );

  MidiMinMax _min_max;

  feature_config_t _input_feature_config;

  run_mode _mode;

  size_t _input_count;
  size_t _input_feature_count;
  size_t _output_count;

  // number of outputs in the LSTM network for each controller
  std::unordered_map<event_data_t,size_t> _ctrl_output_counts;
  // first index in the output for each controller
  std::unordered_map<event_data_t,size_t> _ctrl_output_begin_is;

  ctrl_values_t _ctrl_defaults;
  ctrl_values_t _output_ctrl_values;
  ctrl_values_t _target_ctrl_values;
  std::vector<event_data_t> _note_velocities;
  std::vector<event_data_t> _previous_note_velocities;  

  event_data_t _last_event_type = NO_EVENT;
  event_data_t _last_pitch = NOTE_MAX + 1;
  event_data_t _last_velocity = 0;
  int _last_interval = 0;

  // given a controller and current value, get the index to set to 1.0
  // in the controller's portion of the training target
  std::unordered_map<event_data_t,std::vector<size_t>>
    _ctrl_value_to_hot_index;  
  // given a controller and a hot index from the output, get the
  // controller value
  std::unordered_map<event_data_t,std::vector<event_data_t>>
    _output_to_ctrl_value;

  std::vector<double> _previous_output;
  std::vector<double> _previous_target;  
};

}
