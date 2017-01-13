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

#include "tokens.hpp"

using namespace std;
using namespace larasynth;

string Token::get_type_str() {
  switch( _type ) {
  case INTEGER:
    return "integer";
  case COMMA:
    return "comma";
  case DOT:
    return "dot";
  case MINUS:
    return "minus";
  case L_ANGLE:
    return "left angle bracket";
  case R_ANGLE:
    return "right angle bracket";
  case L_SQUARE:
    return "left square bracket";
  case R_SQUARE:
    return "right square bracket";
  case L_CURLY:
    return "left curly bracket";
  case R_CURLY:
    return "right curly bracket";
  case EQUALS:
    return "equals";
  case COLON:
    return "colon";
  case RESERVED_STRING:
    return "reserved string";
  case QUOTED_STRING:
    return "quoted string";
  case ALPHANUMERIC:
    return "alphanumeric string";
  case END_OF_FILE:
    return "EOF";
  default:
    throw TokenException( "Unknown token type, cannot convert to string" );
  }
}

Tokens::Tokens()
  : _i( 0 )
{}

void Tokens::add( token_t type, string value, string location ) {
  _tokens.emplace_back( type, value, location );
}

string Tokens::get_location() {
  if( _i < _tokens.size() )
    return _tokens[_i].get_location();
  else
    return _tokens.back().get_location();
}

string Tokens::get_prev_location() {
  if( _i == 0 )
    return _tokens[0].get_location();
  else
    return _tokens[_i-1].get_location();
}

string Tokens::eat() {
  if( _i >= _tokens.size() )
    throw TokenException( "Cannot eat, no tokens left" );
  
  return _tokens[_i++].get_value();
}

void Tokens::print() {
  for( auto& token : _tokens ) {
    cout << token.get_type_str() << ": "
         << token.get_value() << " at "
         << token.get_location() << endl;
  }
}

string Tokens::cur_tok_to_str() {
  if( _i == _tokens.size() )
    return "";

  Token& token = _tokens[_i];

  ostringstream oss;
  oss << token.get_type_str() << ": "
      << token.get_value() << " at "
      << token.get_location();

  return oss.str();
}

bool Tokens::match( token_t type ) {
  return ( n_tokens_remaining() > 0 && _tokens[_i].get_type() == type );
}

bool Tokens::match( token_t type_1, token_t type_2 ) {
  return( _tokens.size() > _i + 1 &&
          _tokens[_i].get_type() == type_1 &&
          _tokens[_i+1].get_type() == type_2 );
}

bool Tokens::match( token_t type_1, token_t type_2, token_t type_3 ) {
  return( _tokens.size() > _i + 2 &&
          _tokens[_i].get_type() == type_1 &&
          _tokens[_i+1].get_type() == type_2 &&
          _tokens[_i+2].get_type() == type_3 );         
}

bool Tokens::match( const vector<token_t>& types ) {
  if( _tokens.size() < _i + types.size() )
    return false;

  for( size_t i = 0; i < types.size(); ++i ) {
    if( types[i] != _tokens[_i + i].get_type() )
      return false;
  }

  return true;
}

bool Tokens::match( const string& value ) {
  return ( n_tokens_remaining() > 0 && _tokens[_i].get_value() == value );
}

bool Tokens::match( const vector<string>& values ) {
  if( _tokens.size() < _i + values.size() )
    return false;

  for( size_t i = 0; i < values.size(); ++i ) {
    if( values[i] != _tokens[_i + i].get_value() )
      return false;
  }

  return true;
}
