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
#include <sstream>
#include <vector>

#include "midi_types.hpp"
#include "time_utilities.hpp"

namespace larasynth {

class Event {
public:
  Event();
  Event( const Event& other );
  Event( Event&& other );

  friend void swap( Event& first, Event& second );
  Event& operator=( Event other );

  void set_event( Event* other_ptr );
  void set_event( size_t time, std::vector<event_data_t>& message );
  void set_event( size_t time, event_data_t byte0,
                  event_data_t byte1, event_data_t byte2 );
  void set_event( std::vector<event_data_t>* message );
  void set_event( size_t time, std::vector<event_data_t>* message );  

  event_data_t type() const { return _message[0] & TYPE_MASK; }
  event_data_t channel() const { return _message[0] & CHAN_MASK; }

  event_data_t pitch() const { return _message[1]; }
  event_data_t velocity() const { return _message[2]; }

  event_data_t controller() const { return _message[1]; }
  event_data_t value() const { return _message[2]; }

  size_t time() const { return _time; }
  std::vector<event_data_t>* message_ptr() { return &_message; }
  std::vector<event_data_t> message() { return _message; }

  std::string description( const std::string& label = "" ) const;

  void set_ctrl( event_data_t channel, event_data_t ctrl,
                 event_data_t value, size_t time );

  void set_note_on( event_data_t channel, event_data_t note,
                    event_data_t velocity, size_t time );
  void set_note_off( event_data_t channel, event_data_t note,
                     event_data_t velocity, size_t time );

  void set_controller( event_data_t ctrl ) { _message[1] = ctrl; }
  void set_value( event_data_t value ) { _message[2] = value; }

  void set_pitch( event_data_t pitch ) { _message[1] = pitch; }
  void set_velocity( event_data_t velocity ) { _message[2] = velocity; }

  void set_time( size_t time ) { _time = time; }

protected:
  size_t _time;
  std::vector<event_data_t> _message;
};

}
