/*
Copyright 2017 Nathan Sommer

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

#include <fstream>
#include <vector>
#include <string>

#include "event.hpp"

namespace larasynth {

/** @file */

/**
 * Write events to a training example file.
 *
 * @param events A vector of pointers to Event objects to write.
 * @param filename The name of the file to write to.
 */
void write_events( const std::vector<Event*>& events,
                   const std::string& filename );

/**
 * Write events to a training example file.
 *
 * @param events A vector of Event objects to write.
 * @param filename The name of the file to write to.
 */
void write_events( const std::vector<Event>& events,
                   const std::string& filename );

/**
 * Write a single event to an open file. It is best to call one of the
 * write_events() functions rather than calling this function directly.
 *
 * @param event A pointer to the Event to write.
 * @param outfile An ofstream object associated with the file to write to.
 */
void write_event( const Event* event, std::ofstream& outfile );

}
