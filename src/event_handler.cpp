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

#include "event_handler.hpp"

using namespace std;
using namespace larasynth;

EventHandler::EventHandler( const string& input_port_name,
                            const string& output_port_name, run_mode mode )
  : _client( nullptr )
  , _input_queue( shared_ptr<EventQueue>( new EventQueue ) )
  , _client( "larasynth", input_port_name, output_port_name, _input_queue,
             mode )
{}

void EventHandler::get_next_in_event_copy( Event* event_copy ) {
  Event* next_event = _input_queue->front_pop();
  event_copy->set_event( next_event );
  _input_queue->return_event( next_event );
}
