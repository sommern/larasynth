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

#include <string>
#include <stdexcept>

#include "event.hpp"
#include "run_modes.hpp"

namespace larasynth {

class MidiException : public std::runtime_error {
public:
  explicit MidiException( const std::string& message )
    : std::runtime_error( message ) {};
};

class MidiClient {
public:
  virtual bool has_input_event() =0;
  virtual Event* get_input_event() =0;
  virtual void return_input_event( Event* event ) =0;
  
  virtual void send_event( Event* event ) =0;
};

}
