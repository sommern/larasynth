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
#include <vector>
#include <fstream>

#include "training_sequence.hpp"
#include "event.hpp"
#include "lexer.hpp"
#include "tokens.hpp"

namespace larasynth {

class TrainingSequenceParserException : public std::runtime_error {
public:
  TrainingSequenceParserException( const std::string& message )
    : runtime_error( message ) {};
};

class TrainingSequenceParser {
public:
  TrainingSequenceParser( const std::string& filename );

  TrainingSequence get_sequence() { return _sequence; }

private:
  size_t string_to_size_t( const std::string& str );
  void throw_exception( const std::string& error, const std::string& location );

  void parse_count( Tokens& tokens );
  void parse_note_on( Tokens& tokens );
  void parse_note_off( Tokens& tokens );
  void parse_ctrl( Tokens& tokens );
  
  void check_new_time( size_t time, const std::string& location );

  std::string _filename;
  std::vector<std::string> _lines;

  size_t _prev_time;

  TrainingSequence _sequence;
};

}
