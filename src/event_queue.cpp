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

#include "event_queue.hpp"

using namespace std;
using namespace larasynth;

EventQueue::EventQueue( size_t capacity )
  : RWQLockFreeQueue<Event*>( capacity )
  , _event_pool( capacity )
{}

void EventQueue::push( Event* new_event ) {
  Event* ev = _event_pool.copy_event( new_event );
  RWQLockFreeQueue<Event*>::push( ev );
}

void EventQueue::push( vector<event_data_t>* message ) {
  Event* ev = _event_pool.get_unused_event();
  ev->set_event( message );
  RWQLockFreeQueue<Event*>::push( ev );
}
