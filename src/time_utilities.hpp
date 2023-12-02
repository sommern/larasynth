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

#include <ostream>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace larasynth {

static const size_t MICROSECONDS_PER_MINUTE = 60000000;
static const size_t MICROSECONDS_PER_SECOND = 1000000;
static const size_t MICROSECONDS_PER_MILLISECOND = 1000;

/**
 * Get the current microseconds from the steady clock. This is not in wall
 * time, but represents the number of microseconds from some arbitrary start
 * time.
 */
inline size_t current_microseconds() {
  auto now = std::chrono::steady_clock::now().time_since_epoch();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( now );

  return duration.count();
}

/**
 * Pad a string with zeros on the left.
 */
inline std::string pad_with_zeros( const std::string& string_to_pad,
                                   size_t desired_length = 2) {
  std::string new_string = string_to_pad;

  while( new_string.length() < desired_length )
    new_string = '0' + new_string;

  return new_string;
}

/**
 * Get a timestamp in localtime with microsecond precision.
 */
inline std::string get_timestamp_string() {
  std::ostringstream oss;

  // get timestamp (without microseconds)
  auto rn = std::chrono::system_clock::now();
  std::time_t rnt_t = std::chrono::system_clock::to_time_t(rn);
  std::tm rntm = *std::localtime(&rnt_t);
  // std::tm rntm = *std::gmtime(&rnt_t); // utc

  // calculate microseconds
  auto us = std::chrono::duration_cast<std::chrono::microseconds>(rn.time_since_epoch()) % std::chrono::seconds(1);
  // auto usRounded = std::chrono::round<std::chrono::milliseconds>(us);

  oss << std::put_time(&rntm, "%Y-%m-%d%H.%M.%S.") << us.count();

  return oss.str();
}

/**
 * 
 */
class Timer {
public:
  Timer() {
    start();
  }

  inline void start() { _begin = current_microseconds(); }

  inline size_t get_elapsed_microseconds() {
    return current_microseconds() - _begin;
  }

  inline double get_elapsed_milliseconds() {
    size_t elapsed = get_elapsed_microseconds();
    return (double)elapsed / MICROSECONDS_PER_MILLISECOND;
  }
  
  inline double get_elapsed_seconds() {
    size_t elapsed = get_elapsed_microseconds();
    return (double)elapsed / MICROSECONDS_PER_SECOND;
  }

  inline double get_elapsed_minutes() {
    size_t elapsed = get_elapsed_microseconds();
    return (double)elapsed / MICROSECONDS_PER_MINUTE;
  }

private:
  size_t _begin;
};

}
