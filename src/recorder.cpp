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

#include "recorder.hpp"

using namespace std;
using namespace larasynth;

Recorder::Recorder( const string& filename,
                    const vector<event_data_t>& ctrls,
                    MidiClient* midi_client,
                    volatile sig_atomic_t* shutdown_flag )
  : _filename( filename )
  , _midi_client( midi_client )
  , _ctrls( ctrls )
  , _shutdown_flag( shutdown_flag )
{}

void Recorder::record() {
  size_t note_ons, note_offs;
  note_ons = note_offs = 0;

  vector<Event*> events_to_write;

  cout << "Recording to " << _filename << endl;

  // poll for events
  while( !*_shutdown_flag ) {
    while( _midi_client->has_input_event() ) {
      Event* event = _midi_client->get_input_event();

      if( event->type() == NOTE_ON || event->type() == NOTE_OFF ) {
        if( event->type() == NOTE_OFF ) {
          ++note_offs;
        }
        else if( event->type() == NOTE_ON )
          ++note_ons;

        events_to_write.push_back( event );
      }
      else if( event->type() == CTRL_CHANGE ) {
        for( size_t i = 0; i < _ctrls.size(); ++i ) {
          if( event->controller() == _ctrls[i] ) {
            events_to_write.push_back( event );
            break;
          }
        }
      }
      else
        _midi_client->return_input_event( event );
    }

    // don't hog the CPU
    this_thread::sleep_for( chrono::microseconds( 1 ) );
  }

  cout << endl << "writing " << events_to_write.size() << " events" << endl;

  ofstream outfile( _filename );

  for( Event* event : events_to_write ) {
    write_event( event, outfile );
    _midi_client->return_input_event( event );
  }
}

void Recorder::write_event( Event* event, ofstream& outfile ) {
  ostringstream line_oss;

  switch( event->type() ) {
  case CTRL_CHANGE:
    line_oss << "ctrl " << (unsigned int)event->controller() << " "
             << (unsigned int) event->value() << " " << event->time();
    break;
  case NOTE_ON:
    line_oss << "on " << (unsigned int)event->pitch() << " "
             << (unsigned int)event->velocity() << " "
             << event->time();
    break;
  case NOTE_OFF:
    line_oss << "off " << (unsigned int)event->pitch() << " "
             << event->time();
    break;
  default:
    break;
  }

  outfile << line_oss.str() << endl;;
}
