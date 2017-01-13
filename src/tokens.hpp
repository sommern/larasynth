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
#include <stdexcept>

namespace larasynth {

enum token_t {
  INTEGER,
  COMMA,
  DOT,
  MINUS,
  L_ANGLE,
  R_ANGLE,
  L_SQUARE,
  R_SQUARE,
  L_CURLY,
  R_CURLY,
  EQUALS,
  COLON,
  RESERVED_STRING,
  QUOTED_STRING,
  ALPHANUMERIC,
  SECTION_HEADER,
  END_OF_FILE
};

class TokenException : public std::runtime_error {
public:
  TokenException( const std::string& message ) : runtime_error( message ) {};
};

class Token {
public:
  Token( token_t type, std::string value, std::string location )
    : _type( type ),
      _value( value ),
      _location( location )
  {}

  token_t get_type() { return _type; }
  std::string get_type_str();
  std::string get_value() { return _value; }
  std::string get_location() { return _location; }

private:
  token_t _type;
  std::string _value;
  std::string _location;
};

class Tokens {
public:
  Tokens();

  void add( token_t type, std::string value, std::string location );
  void add( const Token& token ) { _tokens.push_back( token ); }
  size_t size() { return _tokens.size(); }
  size_t n_tokens_remaining() { return _tokens.size() - _i; }
  std::string get_location();
  std::string get_prev_location();  
  std::string cur_tok_to_str();

  bool match( token_t type );
  bool match( token_t type_1, token_t type_2 );
  bool match( token_t type_1, token_t type_2, token_t type_3 );  
  bool match( const std::vector<token_t>& types );
  bool match( const std::string& value );
  bool match( const std::vector<std::string>& values );  

  std::string eat();

  void print();

private:
  std::vector<Token> _tokens;
  size_t _i;
};

}
