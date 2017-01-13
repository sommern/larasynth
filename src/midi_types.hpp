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

#include <map>
#include <unordered_map>
#include <string>

namespace larasynth {

typedef unsigned char event_data_t;

typedef std::unordered_map<event_data_t,event_data_t> ctrl_values_t;

struct midi_range {
  event_data_t min;
  event_data_t max;
};

typedef std::map<size_t,midi_range> midi_range_map_t;

#define TYPE_MASK 0xf0
#define CHAN_MASK 0xf

enum event_type_t {
  NOTE_ON = 0x90,
  NOTE_OFF = 0x80,
  CTRL_CHANGE = 0xb0,
  PITCH_BEND = 0xe0,
  NO_EVENT = 0x00
};

#define EVENT_DATA_SIZE 3

#define NOTE_MAX 127
#define NOTE_MIN 0

#define VEL_MAX 127
#define VEL_MIN 0

#define CTRL_MAX 127
#define CTRL_MIN 0

inline std::string event_type_to_string( event_data_t type ) {
  switch( type ) {
  case NOTE_ON:
    return "note on";
  case NOTE_OFF:
    return "note off";
  case CTRL_CHANGE:
    return "ctrl change";
  case PITCH_BEND:
    return "pitch bend";
  default:
    return "invalid type";
  }
}

}
