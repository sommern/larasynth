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
#include <unordered_map>
#include <bitset>

namespace larasynth {

typedef unsigned int input_feature_t;

// OS X clang does not like maps using enums as keys
static const input_feature_t SOME_NOTE_ON = 0;
static const input_feature_t NOTE_STRUCK = 1;
static const input_feature_t NOTE_RELEASED = 2;
static const input_feature_t VELOCITY = 3;
static const input_feature_t INTERVAL = 4;

static const std::vector<input_feature_t> input_feature_list = {
  SOME_NOTE_ON,
  NOTE_STRUCK,
  NOTE_RELEASED,
  VELOCITY,
  INTERVAL
};

static const std::unordered_map<input_feature_t,size_t> input_feature_sizes {
  { SOME_NOTE_ON, 1 },
  { NOTE_STRUCK, 1 },
  { NOTE_RELEASED, 1 },
  { VELOCITY, 1 },
  // twelve for the number of semitones, 2 for the direction, 2 for more than
  // an octave away
  { INTERVAL, 12 + 2 + 2 }
};  

static const std::unordered_map<std::string,input_feature_t>
feature_string_to_type = {
   { "some note on", SOME_NOTE_ON },
   { "note struck", NOTE_STRUCK },
   { "note released", NOTE_RELEASED },
   { "velocity", VELOCITY },
   { "interval", INTERVAL }
};

typedef std::bitset<5> feature_config_t;

}
