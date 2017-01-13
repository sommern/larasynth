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
#include <iostream>

#include "training_sequence.hpp"
#include "midi_types.hpp"
#include "event.hpp"

namespace larasynth {

class MidiMinMax {
public:
  MidiMinMax();

  bool operator==( const MidiMinMax &other ) const {
    return( other._note_min == _note_min && other._note_max == _note_max &&
            other._ctrl_mins == _ctrl_mins && other._ctrl_maxs == _ctrl_maxs );
  }

  void consider_sequence( const TrainingSequence& seq );

  event_data_t get_note_min() const { return _note_min; }
  event_data_t get_note_max() const { return _note_max; }

  std::vector<event_data_t> get_ctrls();

  event_data_t get_ctrl_min( event_data_t ctrl ) const
  { return _ctrl_mins[ctrl]; }
  event_data_t get_ctrl_max( event_data_t ctrl ) const
  { return _ctrl_maxs[ctrl]; }

  void set_note_min( event_data_t min ) { _note_min = min; }
  void set_note_max( event_data_t max ) { _note_max = max; }  

  void set_ctrl_min( event_data_t ctrl, event_data_t min )
  { _ctrl_mins[ctrl] = min; }
  void set_ctrl_max( event_data_t ctrl, event_data_t max )
  { _ctrl_maxs[ctrl] = max; }

  friend std::ostream& operator<<( std::ostream& os,
                                   const MidiMinMax& min_max );

private:
  event_data_t _note_min;
  event_data_t _note_max;

  std::vector<event_data_t> _ctrl_mins;
  std::vector<event_data_t> _ctrl_maxs;
};

}
