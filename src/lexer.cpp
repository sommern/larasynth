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

#include "lexer.hpp"

using namespace std;
using namespace larasynth;

Lexer::Lexer( const string& filename, vector<string> reserved_strings )
  : _filename( filename ),
    _reserved_strings( reserved_strings )
{
  read_file();
  tokenize();
}

void Lexer::read_file() {
  ifstream file( _filename.c_str() );

  if( !file )
    throw LexerException( "Error opening " + _filename );

  string line;
  while( getline( file, line ) )
    _lines.push_back( line );
}

void Lexer::tokenize() {
  size_t line_number = 0;
  for( string line : _lines ) {
    ++line_number;
    tokenize_line( line, line_number );
  }
}

void Lexer::tokenize_line( string line, size_t line_number ) {
  ostringstream loc_oss;
  loc_oss << _filename << ":" << line_number;
  string location = loc_oss.str();

  size_t pos = line.find( '#' );
  if( pos != string::npos )
    line.erase( pos );

  regex beginning_whitespace_re( "^\\s+" );
  regex integer_re( "^(\\d+)" );
  regex quoted_string_re( "^\"(([^\"\\\\]|\\\\.)*)\"" );
  regex alphanumeric_re( "^(\\w+)" );

  smatch m;

  size_t token_count = 0;
  while( line.length() > 0 ) {
    // erase leading whitespace
    line = regex_replace( line, beginning_whitespace_re, "" );
    if( line.length() == 0 )
      break;

    // check for an integer
    if( regex_search( line, m, integer_re ) ) {
      _tokens.add( INTEGER, m[1], location );
      line = regex_replace( line, integer_re, "" );
      continue;
    }

    // check for symbols
    switch( line[0] ) {
    case ',':
      _tokens.add( COMMA, ",", location );
      line.erase( 0, 1 );
      break;
    case '.':
      _tokens.add( DOT, ".", location );
      line.erase( 0, 1 );
      break;
    case '-':
      _tokens.add( MINUS, "-", location );
      line.erase( 0, 1 );
      break;
    case '[':
      _tokens.add( L_SQUARE, "[", location );
      line.erase( 0, 1 );
      break;
    case ']':
      _tokens.add( R_SQUARE, "]", location );
      line.erase( 0, 1 );
      break;
    case '{':
      _tokens.add( L_CURLY, "[", location );
      line.erase( 0, 1 );
      break;
    case '}':
      _tokens.add( R_CURLY, "]", location );
      line.erase( 0, 1 );
      break;
    case '=':
      _tokens.add( EQUALS, "=", location );
      line.erase( 0, 1 );
      break;
    case ':':
      _tokens.add( COLON, ":", location );
      line.erase( 0, 1 );
      break;
    default:
      break;
    }

    if( _tokens.size() > token_count ) {
      token_count = _tokens.size();
      continue;
    }

    // check for reserved string
    for( string& str : _reserved_strings ) {
      if( line.find( str ) == 0 ) {
        _tokens.add( RESERVED_STRING, str, location );
        line.erase( 0, str.length() );
        break;
      }
    }

    if( _tokens.size() > token_count ) {
      token_count = _tokens.size();
      continue;
    }

    // check for quoted string
    if( regex_search( line, m, quoted_string_re ) ) {
      string quoted = m[1];

      while( true ) {
        size_t pos = quoted.find( "\\\"" );

        if( pos == string::npos )
          break;

        quoted.replace( pos, 2, "\"" );
      }

      _tokens.add( QUOTED_STRING, quoted, location );
      line = regex_replace( line, quoted_string_re, "" );
      continue;
    }

    // check for alphanumeric string
    if( regex_search( line, m, alphanumeric_re ) ) {
      _tokens.add( ALPHANUMERIC, m[1], location );
      line = regex_replace( line, alphanumeric_re, "" );
      continue;
    }

    ostringstream oss;
    oss << "Unexpected symbol '" << line[0] << "'";
    throw TokenException( oss.str() );
  }
}
