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

#include "midi_min_max.hpp"

using namespace std;
using namespace larasynth;

MidiMinMax::MidiMinMax()
  : _note_min( 0 )
  , _note_max( 127 )
  , _ctrl_mins( vector<event_data_t>( 128, 127 ) )
  , _ctrl_maxs( vector<event_data_t>( 128, 0 ) )
{}

/**
 * Get all the controller numbers that have had their min and max set.
 *
 * If the controller's min and max were not set, the min is 127 and the max is
 * 0.
 */
vector<event_data_t> MidiMinMax::get_ctrls() {
  vector<event_data_t> ctrls;

  for( size_t ctrl = 0; ctrl < _ctrl_mins.size(); ++ctrl ) {
    event_data_t min = _ctrl_mins[ctrl];
    event_data_t max = _ctrl_maxs[ctrl];

    if( min <= max )
      ctrls.push_back( ctrl );
  }

  return ctrls;
}

void MidiMinMax::consider_sequence( const TrainingSequence& seq ) {
  for( auto& event : seq.get_events_ref() ) {
    switch( event.type() ) {
    case NOTE_ON:
      if( event.pitch() > _note_max )
        _note_max = event.pitch();
      else if( event.pitch() < _note_min )
        _note_min = event.pitch();
      break;
    case CTRL_CHANGE:
      if( event.value() > _ctrl_maxs[event.controller()] )
        _ctrl_maxs[event.controller()] = event.value();
      if( event.value() < _ctrl_mins[event.controller()] )
        _ctrl_mins[event.controller()] = event.value();
      break;
    }
  }
}

namespace larasynth {
  ostream& operator<<( ostream& os, const MidiMinMax& min_max ) {
    os << "Note min: " << (unsigned int)min_max._note_min << endl;
    os << "Note max: " << (unsigned int)min_max._note_max << endl;

    for( size_t i = 0; i < min_max._ctrl_mins.size(); ++i ) {
      event_data_t min = min_max._ctrl_mins[i];
      event_data_t max = min_max._ctrl_maxs[i];

      if( min < max ) {
        cout << "Ctrl " << i << " min: " << (unsigned int)min << endl;
        cout << "Ctrl " << i << " max: " << (unsigned int)max << endl;
      }
    }
  
    return os;
  }
}
