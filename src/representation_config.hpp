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

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>

#include "midi_types.hpp"
#include "config_parameter.hpp"
#include "config_parameters.hpp"
#include "representation_defaults.hpp"
#include "input_features.hpp"

namespace larasynth {

class RepresentationConfigException : public std::runtime_error {
public:
  explicit RepresentationConfigException( const std::string& message )
    : runtime_error( "Error in configuration section [representation]:\n" +
                     message ) {};
};

class RepresentationConfig {
public:
  RepresentationConfig( std::vector<size_t> ctrl_output_counts_list,
                        size_t update_rate,
                        feature_config_t input_feature_config );
  explicit RepresentationConfig( ConfigParameters& config_params );

  void print_representation_configuration();

  std::unordered_map<event_data_t, size_t> get_ctrl_output_counts() const
  { return _ctrl_output_counts; }
  size_t get_ctrl_output_count( event_data_t ctrl ) const
  { return _ctrl_output_counts.at( ctrl ); }
  size_t get_total_output_count() const;

  size_t get_update_rate() const { return _update_rate; }
  feature_config_t get_input_feature_config() const
  { return _input_feature_config; }
  bool use_feature_some_note_on() const
  { return _input_feature_config[SOME_NOTE_ON]; }
  bool use_feature_note_struck() const
  { return _input_feature_config[NOTE_STRUCK]; }
  bool use_feature_note_released() const
  { return _input_feature_config[NOTE_RELEASED]; }
  bool use_feature_velocity() const
  { return _input_feature_config[VELOCITY]; }
  bool use_feature_interval() const
  { return _input_feature_config[INTERVAL]; }

private:
  std::unordered_map<event_data_t, size_t> _ctrl_output_counts;

  size_t _update_rate;

  feature_config_t _input_feature_config;
};

}
