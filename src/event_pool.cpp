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

#include "event_pool.hpp"

using namespace larasynth;

EventPool::EventPool( size_t capacity )
  : _rb( capacity )
{
  for( size_t i = 0; i < capacity / 2; ++i )
    _rb.push( new Event );
}

EventPool::~EventPool() {
  while( !_rb.empty() ) {
    delete _rb.front_pop();
  }
}

Event* EventPool::get_unused_event() {
  if( _rb.empty() ) {
    return new Event;
  }
  else {
    Event* ev = _rb.front_pop();
    return ev;
  }
}

void EventPool::return_event( Event* event ) {
  _rb.push_or_delete_if_full( event );
}

Event* EventPool::copy_event( Event* ev ) {
  Event* new_event = get_unused_event();
  new_event->set_event( ev );

  return new_event;
}
