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

#include "midi_state.hpp"

using namespace std;
using namespace larasynth;

MidiState::MidiState( const ctrl_values_t& ctrl_values )
  : _ctrl_defaults( ctrl_values )
  , _ctrl_values( ctrl_values )
  , _note_velocities( 128, 0 )
{}

void MidiState::reset() {
  _ctrl_values = _ctrl_defaults;
}

void MidiState::set_ctrl_value( event_data_t ctrl, event_data_t value ) {
  _ctrl_values[ctrl] = value;
}

bool MidiState::any_notes_on() const {
  size_t sum = accumulate( _note_velocities.begin(), _note_velocities.end(),
                           0 );

  return sum > 0;
}

void MidiState::new_event( const Event& event ) {
  if( event.type() == NOTE_ON && event.velocity() != 0 )
    _note_velocities[event.pitch()] = event.velocity();
  else if( event.type() == NOTE_OFF ||
           ( event.type() == NOTE_ON && event.velocity() == 0 ) )
    _note_velocities[event.pitch()] = 0;
  else if( event.type() == CTRL_CHANGE )
    _ctrl_values[event.controller()] = event.value();
}

namespace larasynth {
ostream& operator<<( ostream& os, const MidiState& state ) {
  if( state.any_notes_on() ) {
    os << "Notes on: " << endl;;

    for( size_t note = 0; note < state._note_velocities.size(); ++note ) {
      if( state._note_velocities[note] > 0 )
        os << note << ": " << (unsigned int)state._note_velocities[note]
           << endl;
    }
  }
  else
    os << "No notes on" << endl;

  os << "Controller values:" << endl;

  for( auto& kv : state._ctrl_values )
    os << (unsigned int)kv.first << ": " << (unsigned int)kv.second << endl;

  return os;
}
}
