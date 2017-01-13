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

#include <random>
#include <functional>
#include <algorithm>
#include <array>

namespace larasynth {

enum random_type { UNIFORM, NORMAL };

class RandGen {
public:
  RandGen() {
    // we'll use a seed sequence whose length is the same as the size of the
    // RNG engine
    const auto seed_seq_length = std::mt19937::state_size;

    std::mt19937::result_type seed_data[seed_seq_length];

    std::random_device source;

    // fill the array with seed_seq_length integers from the random device
    std::generate( std::begin( seed_data ), std::end( seed_data ),
                   std::ref( source ) );

    std::seed_seq seeds( std::begin( seed_data ), std::end( seed_data ) );

    _rand_engine = std::mt19937( seeds );
  }

  template <typename T>
  T normal( T mean, T std ) {
    std::normal_distribution<T> dist( mean, std );
    return dist( _rand_engine );
  }

  template <typename T>
  T uniform_real( T min, T max ) {
    std::uniform_real_distribution<T> dist( min, max );
    return dist( _rand_engine );
  }

  template <typename T>
  T uniform_int( T min, T max ) {
    std::uniform_int_distribution<T> dist( min, max );
    return dist( _rand_engine );
  }

  std::mt19937* get_engine_ptr() {
    return &_rand_engine;
  }

private:
  //std::random_device _rd;
  std::mt19937 _rand_engine;
};

}
