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

#include <stdexcept>
#include <string>

namespace larasynth {

class LockFreeQueueException : std::runtime_error {
public:
  explicit LockFreeQueueException( const std::string& message )
    : std::runtime_error( message ) {};
};

template<class T>
class LockFreeQueue {
public:
  virtual bool empty() =0;
  virtual void pop() =0;
  virtual T front() =0;
  virtual T front_pop() =0;
  virtual void push( T item ) =0;
  virtual void push_or_delete_if_full( T item ) =0;
};

}
