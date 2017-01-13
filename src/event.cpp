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

#include "event.hpp"

using namespace std;
using namespace larasynth;

Event::Event()
  : _time( 0 ),
    _message( EVENT_DATA_SIZE, 0 )
{}

Event::Event( const Event& other )
  : _time( other._time ),
    _message( other._message )
{}

Event::Event( Event&& other )
  : Event()
{
  swap( *this, other );
}

namespace larasynth {
  void swap( Event& first, Event& second ) {
    using std::swap;

    swap( first._time, second._time );
    swap( first._message, second._message );
  }
}

Event& Event::operator=( Event other ) {
  swap( *this, other );
  return *this;
}

void Event::set_event( Event* other_ptr ) {
  _time = other_ptr->time();
  _message = other_ptr->message();
}

void Event::set_event( size_t time, vector<event_data_t>* message ) {
  _time = time;

  _message = *message;
}

void Event::set_event( size_t time, event_data_t byte0,
                       event_data_t byte1, event_data_t byte2 ) {
  _time = time;

  _message[0] = byte0;
  _message[1] = byte1;
  _message[2] = byte2;
}

void Event::set_event( vector<event_data_t>* message ) {
  _time = current_microseconds();

  copy( message->begin(), message->end(), _message.begin() );

  _message = *message;
}

void Event::set_ctrl( event_data_t channel, event_data_t ctrl,
                      event_data_t value, size_t time ) {
  _message[0] = CTRL_CHANGE | channel;
  _message[1] = ctrl;
  _message[2] = value;
  _time = time;
}

void Event::set_note_on( event_data_t channel, event_data_t note,
                         event_data_t velocity, size_t time ) {
  _message[0] = NOTE_ON | channel;
  _message[1] = note;
  _message[2] = velocity;
  _time = time;
}

string Event::description( const string& label ) const {
  ostringstream oss;

  oss << label << " t: " << _time << ", ";

  unsigned int type = (unsigned int)this->type();
  unsigned int channel = (unsigned int)this->channel();
  unsigned int pb_value;

  switch( type ) {
  case NOTE_ON:
    oss << " ON  " << (unsigned int)_message[1] << " "
        << (unsigned int)_message[2];
    break;
  case NOTE_OFF:
    oss << " OFF " << (unsigned int)_message[1] << " "
        << (unsigned int)_message[2];
    break;
  case CTRL_CHANGE:
    oss << "control change, channel " << channel << ": controller "
        << (unsigned int)_message[1] << ", value "
        << (unsigned int)_message[2];
    break;
  case PITCH_BEND:
    pb_value = _message[1] | ( _message[2] << 7 );
    oss << "pitch bend, channel " << channel << ": value " << pb_value;
    break;
  default:
    oss << "unknown event type " << type;
    break;
  }

  return oss.str();
}
