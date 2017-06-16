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

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include <cassert>
#include <set>

#include "littlelstm/lstm_types.hpp"
#include "config_parameters.hpp"
#include "midi_types.hpp"

namespace larasynth {

class MidiConfigException : public std::runtime_error {
public:
  MidiConfigException( const std::string& message )
    : runtime_error( "Error in configuration section [midi]:\n" + message ) {};
};

class MidiConfig {
public:
  MidiConfig( ConfigParameters& config_params );

  std::string get_recording_source_port() { return _recording_source_port; }
  std::string get_performing_source_port() { return _performing_source_port; }
  std::string get_performing_destination_port()
  { return _performing_destination_port; }

  std::vector<event_data_t> get_ctrls() { return _ctrls; }

  bool is_ctrl( event_data_t ctrl )
  { return find( _ctrls.begin(), _ctrls.end(), ctrl ) != _ctrls.end(); }

  ctrl_values_t get_ctrl_defaults() const
  { assert( _ctrl_defaults.size() > 0 ); return _ctrl_defaults; }
  event_data_t get_ctrl_default( event_data_t ctrl )
  { return _ctrl_defaults[ctrl]; }

private:

  std::string _recording_source_port;
  std::string _performing_source_port;
  std::string _performing_destination_port;  

  std::vector<event_data_t> _ctrls;
  ctrl_values_t _ctrl_defaults;
};

}
