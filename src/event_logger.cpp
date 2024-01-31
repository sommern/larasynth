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

#include <thread>

#include "event_logger.hpp"

using namespace std;
using namespace larasynth;

EventLogger::EventLogger( string label, size_t capacity )
  : RWQLockFreeQueue<Event*>( capacity )
  , EventPool( capacity )
  , _label( label )
  , _run( true )
  , _logger( &EventLogger::run_logger, this )
{}

EventLogger::~EventLogger() {
  _run = false;
  _logger.join();
  //cout << "Event logger shut down\n";
}

void EventLogger::log_event( Event* event ) {
  push( copy_event( event ) );
}

void EventLogger::run_logger() {
  while( _run ) {
    while( !empty() ) {
      Event* e = front_pop();
      cout << e->description( _label ) << endl;
      return_event( e );
    }
    std::this_thread::sleep_for( std::chrono::microseconds( 10000 ) );
  }
}

