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
#include <unordered_map>
#include <numeric>
#include <stdexcept>

#include "event.hpp"
#include "midi_types.hpp"

namespace larasynth {

class MidiStateException : public std::runtime_error {
public:
  explicit MidiStateException( const std::string& message )
    : runtime_error( message ) {};
};

class MidiState {
public:
  MidiState( const ctrl_values_t& ctrl_values );

  void new_event( const Event& event );

  void reset();

  event_data_t get_ctrl_value( event_data_t ctrl ) const
  { return _ctrl_values.at( ctrl ); }
  ctrl_values_t get_ctrl_values() const
  { return _ctrl_values; }
  std::vector<event_data_t> get_note_velocities() const
  { return _note_velocities; }
  bool any_notes_on() const;

  void set_ctrl_value( event_data_t ctrl, event_data_t value );

  friend std::ostream& operator<<( std::ostream& os,
                                   const MidiState& state );

private:
  ctrl_values_t _ctrl_defaults;
  ctrl_values_t _ctrl_values;

  std::vector<event_data_t> _note_velocities;
};

}
