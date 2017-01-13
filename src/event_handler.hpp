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

#include "event_queue.hpp"
#include "rtmidi_client.hpppp"
#include "run_modes.hpp"

namespace larasynth {

class EventHandler {
public:
  EventHandler( const std::string& input_port_name,
                const std::string& output_port_name, run_mode mode );

  bool is_new_in_event() { return !_input_queue->empty(); }
  Event* get_next_in_event() { return _input_queue->front_pop(); }
  void get_next_in_event_copy( Event* event_copy );
  void return_in_event( Event* event ) { _input_queue->return_event( event ); }
  
  void send_out_event( Event* event ) { _client.send_out_event( event ); }

protected:
  std::shared_ptr<EventQueue> _input_queue;
  RtMidiClient _client;
};

}
