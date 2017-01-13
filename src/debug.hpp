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

//#define NODEBUG

#include <cassert>

#ifndef NODEBUG

#define LOC std::cout << "debug:" << __FILE__ << ":" << __LINE__ << " ";

#define DEBUG_COUT(text) LOC std::cout << text << std::endl;
#define DEBUG_VAR(text) LOC std::cout << (#text) << "=" << text << std::endl;
#define DEBUG(text) LOC text
#define DEBUGQ(text) text
 
#else

#define DEBUG_COUT(text)
#define DEBUG_VAR(text)
#define DEBUG(text)
#define DEBUGQ(text)

#endif
