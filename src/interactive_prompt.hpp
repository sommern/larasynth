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

#include <iostream>
#include <string>
#include <locale>
#include <vector>
#include <cctype>
#include <sstream>

namespace larasynth {

struct ip_choice_t {
  ip_choice_t( const std::string& i, const std::string& d )
    : choice_input( i )
    , description( d )
  {}

  std::string choice_input;
  std::string description;
};

bool pick_yes_or_no( const std::string& question, char default_choice = 'x' );
size_t pick_choice( const std::string& question,
                    const std::vector< ip_choice_t >& choices,
                    int default_choice = -1 );
size_t get_positive_integer( std::string& question );

}
