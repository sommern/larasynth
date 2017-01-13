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
#include <sstream>
#include <fstream>
#include <regex>
#include <stdexcept>

#include "tokens.hpp"

namespace larasynth {

class LexerException : public std::runtime_error {
public:
  LexerException( const std::string& message )
    : runtime_error( message ) {};
};

class Lexer {
public:
  Lexer( const std::string& filename,
         std::vector<std::string> reserved_strings =
         std::vector<std::string>() );

  Tokens get_tokens() { return _tokens; }
  std::vector<std::string> get_lines() { return _lines; }

private:
  void read_file();
  void tokenize();
  void tokenize_line( std::string line, size_t location );

  std::string _filename;
  std::vector<std::string> _reserved_strings;

  std::vector<std::string> _lines;

  Tokens _tokens;
};

}
