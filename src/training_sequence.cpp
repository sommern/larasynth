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

#include "training_sequence.hpp"

using namespace std;
using namespace larasynth;

TrainingSequence::TrainingSequence()
  : _count_min( 1 ),
    _count_max( 1 )
{}

TrainingSequence::TrainingSequence( const std::vector<Event>& events )
  : _events( events )
  , _count_min( 1 )
  , _count_max( 1 )
{}

TrainingSequence::TrainingSequence( const TrainingSequence& other )
  : _events( other.get_events() )
  , _count_min( other.get_count_min() )
  , _count_max( other.get_count_max() )
{}
