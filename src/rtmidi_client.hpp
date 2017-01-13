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

#include <cassert>
#include <string>
#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <stdexcept>

#include "midi_types.hpp"
#include "event.hpp"
#include "event_queue.hpp"
#include "run_modes.hpp"
#include "midi_client.hpp"
#include "rtmidi/RtMidi.h"
#include "interactive_prompt.hpp"

namespace larasynth {

void input_callback( double delta_time, std::vector<unsigned char>* message,
                     void* in_q );

class RtMidiClient : public MidiClient {
public:
  RtMidiClient( const std::string& client_name,
                const std::string& input_port_name,
                const std::string& output_port_name,
                run_mode mode );
  ~RtMidiClient();

  bool has_input_event() { return !_input_queue.empty(); }
  Event* get_input_event() { return _input_queue.front_pop(); }
  void return_input_event( Event* event )
  { _input_queue.return_event( event ); }

  void send_event( Event* event )
  { _midi_out->sendMessage( event->message_ptr() ); }

private:
  void setup_midi_in();
  void setup_midi_out();

  void throw_exception( std::string error_msg );

  RtMidiIn* _midi_in;
  RtMidiOut* _midi_out;

  std::string _client_name;

  std::string _input_port_name;
  std::string _output_port_name;

  EventQueue _input_queue;
  
  run_mode _mode;
};

}
