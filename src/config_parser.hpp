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
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <stdexcept>
#include <regex>

#include "config_parameter.hpp"
#include "config_parameters.hpp"
#include "lexer.hpp"
#include "tokens.hpp"
#include "rand_gen.hpp"
#include "debug.hpp"

namespace larasynth {

class ConfigParserException : public std::runtime_error {
public:
  ConfigParserException( const std::string& message )
    : runtime_error( message ) {};
};

class ConfigParser {
public:
  ConfigParser( std::string filename );

  bool section_exists( const std::string& name );
  ConfigParameters get_section_params( const std::string& name );

private:
  void read_lines();
  void parse();
  bool real_match( Tokens& tokens );
  bool int_match( Tokens& tokens );
  std::string parse_section_name( Tokens& tokens );
  ConfigParameter parse_right_side( Tokens& tokens );
  void parse_real_list( std::vector<double>& list, Tokens& tokens );
  void parse_int_list( std::vector<int>& list, Tokens& tokens );
  void parse_string_list( std::vector<std::string>& list, Tokens& tokens );

  void throw_exception( const std::string& error, const std::string& location );
  void throw_wrong_rand_list( random_type rand_type,
                              const std::string& location );


  std::string _filename;
  std::vector<std::string> _lines;
  std::map<std::string,ConfigParameters> _section_parameters;

  RandGen _rand_gen;
};

}
