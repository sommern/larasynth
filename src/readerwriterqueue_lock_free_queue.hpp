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

#include "lock_free_queue.hpp"
#include "readerwriterqueue/readerwriterqueue.h"

namespace larasynth {

template<class T>
class RWQLockFreeQueue : public LockFreeQueue<T> {
public:
  explicit RWQLockFreeQueue( size_t capacity = 1024 );
  ~RWQLockFreeQueue();

  bool empty() { return _q.peek() == nullptr; }

  void pop();
  T front();
  T front_pop();
  void push( T item );
  void push_or_delete_if_full( T item );

private:
  size_t _capacity;
  moodycamel::ReaderWriterQueue<T> _q;
};

template <class T>
RWQLockFreeQueue<T>::RWQLockFreeQueue( size_t capacity )
  : _capacity( capacity ),
    _q( capacity )
{}

template <class T>
RWQLockFreeQueue<T>::~RWQLockFreeQueue() {
  while( !empty() ) {
    T item = front_pop();
    delete item;
  }
}

template <class T>
void RWQLockFreeQueue<T>::pop() {
  front_pop();
}

template <class T>
T RWQLockFreeQueue<T>::front() {
  T* data_ptr;
  data_ptr = _q.peek();
  if( data_ptr == nullptr )
    throw LockFreeQueueException( "Attempt to access an empty queue" );

  return *data_ptr;
}

template <class T>
T RWQLockFreeQueue<T>::front_pop() {
  T data;
  if( !_q.try_dequeue( data ) )
    throw LockFreeQueueException( "Attempt to access an empty queue" );
  
  return data;
}

template <class T>
void RWQLockFreeQueue<T>::push( T item ) {
  _q.enqueue( item );
}

template <class T>
void RWQLockFreeQueue<T>::push_or_delete_if_full( T item ) {
  if( !_q.try_enqueue(item) )
    delete item;
}

}
