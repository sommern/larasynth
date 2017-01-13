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

#include "readerwriterqueue_lock_free_queue.hpp"
#include "event.hpp"

namespace larasynth {

class EventPool {
public:
  EventPool( size_t capacity = 2048 );
  ~EventPool();

  Event* get_unused_event();
  void return_event( Event *event );

  Event* copy_event( Event* event );

private:
  RWQLockFreeQueue<Event*> _rb;
};

}
