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

#include <vector>

#include "event.hpp"

namespace larasynth {

class TrainingSequence {
public:
  TrainingSequence();
  TrainingSequence( const std::vector<Event>& events );
  TrainingSequence( const TrainingSequence& other );
  
  void add_event( Event event ) { _events.push_back( event ); }

  void set_count_min( std::size_t count ) { _count_min = count; }
  void set_count_max( std::size_t count ) { _count_max = count; }

  std::size_t get_count_min() const { return _count_min; }
  std::size_t get_count_max() const { return _count_max; }  
  
  std::vector<Event> get_events() const { return _events; }

  const std::vector<Event>& get_events_ref() const { return _events; }

private:
  std::vector<Event> _events;

  std::size_t _count_min;
  std::size_t _count_max;
};

}
