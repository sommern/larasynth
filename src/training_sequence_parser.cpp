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

#include "training_sequence_parser.hpp"

using namespace std;
using namespace larasynth;

TrainingSequenceParser::TrainingSequenceParser( const string& filename )
  : _filename( filename ),
    _prev_time( 0 ) {
  vector<string> reserved_strings = { "count:", "on", "off", "ctrl" };

  Lexer lexer( filename, reserved_strings );

  Tokens tokens = lexer.get_tokens();

  while( tokens.n_tokens_remaining() > 0 ) {
    if( tokens.match( RESERVED_STRING ) ) {
      string s = tokens.eat();

      if( s == "count:" )
        parse_count( tokens );
      else if( s == "on" )
        parse_note_on( tokens );
      else if( s == "off" )
        parse_note_off( tokens );
      else if( s == "ctrl" )
        parse_ctrl( tokens );
      else
        throw_exception( "unexpected string '" + s + "'",
                         tokens.get_prev_location() );
    }
    else {
      string s = tokens.eat();
      throw_exception( "unexpected string '" + s + "'",
                       tokens.get_prev_location() );
    }
  }
}

void TrainingSequenceParser::parse_count( Tokens& tokens ) {
  if( tokens.match( L_SQUARE ) ) {
    tokens.eat();

    if( !tokens.match( INTEGER ) ) {
      throw_exception( "expected an integer after '[' in 'count:'",
                       tokens.get_location() );
    }

    _sequence.set_count_min( string_to_size_t( tokens.eat() ) );

    if( !tokens.match( COMMA ) ) {
      throw_exception( "expected a comma after integer in 'count:'",
                       tokens.get_location() );
    }

    tokens.eat();

    if( !tokens.match( INTEGER ) ) {
      throw_exception( "expected an integer after ',' in 'count:'",
                       tokens.get_location() );
    }

    _sequence.set_count_max( string_to_size_t( tokens.eat() ) );

    if( !tokens.match( R_SQUARE ) ) {
      throw_exception( "expected ']'", tokens.get_location() );
    }

    tokens.eat();

    if( _sequence.get_count_max() < _sequence.get_count_min() )
      throw_exception( "Count count min must be less than count count max",
                       tokens.get_prev_location() );
  }

  else {
    if( !tokens.match( INTEGER ) ) {
      throw_exception( "expected integer count count",
                       tokens.get_location() );
    }

    size_t count = string_to_size_t( tokens.eat() );

    _sequence.set_count_min( count );
    _sequence.set_count_max( count );
  }
}

void TrainingSequenceParser::parse_note_on( Tokens& tokens ) {
  if( !tokens.match( INTEGER ) )
    throw_exception( "Expected integer note number", tokens.get_location() );

  size_t note_num = string_to_size_t( tokens.eat() );

  if( note_num > 127 )
    throw_exception( "Note number must be < 128", tokens.get_location() );

  if( !tokens.match( INTEGER ) )
    throw_exception( "Expected integer velocity", tokens.get_location() );

  size_t velocity = string_to_size_t( tokens.eat() );

  if( velocity > 127 )
    throw_exception( "Velocity must be < 128", tokens.get_location() );

  if( !tokens.match( INTEGER ) )
    throw_exception( "Expected integer time", tokens.get_location() );

  size_t time = string_to_size_t( tokens.eat() );

  check_new_time( time, tokens.get_prev_location() );

  Event event;

  event.set_note_on( 0, note_num, velocity, time );

  _sequence.add_event( event );
}

void TrainingSequenceParser::parse_note_off( Tokens& tokens ) {
  if( !tokens.match( INTEGER ) )
    throw_exception( "Expected integer note number", tokens.get_location() );

  size_t note_num = string_to_size_t( tokens.eat() );

  if( note_num > 127 )
    throw_exception( "Note number must be < 128", tokens.get_location() );

  if( !tokens.match( INTEGER ) )
    throw_exception( "Expected integer time", tokens.get_location() );

  size_t time = string_to_size_t( tokens.eat() );

  check_new_time( time, tokens.get_prev_location() );

  Event event;

  event.set_note_on( 0, note_num, 0, time );

  _sequence.add_event( event );
}

void TrainingSequenceParser::parse_ctrl( Tokens& tokens ) {
  if( !tokens.match( INTEGER ) )
    throw_exception( "Expected integer controller", tokens.get_location() );

  size_t ctrl_num = string_to_size_t( tokens.eat() );

  if( ctrl_num > 127 )
    throw_exception( "Controller must be < 128", tokens.get_prev_location() );

  if( !tokens.match( INTEGER ) )
    throw_exception( "Expected integer controller value",
                     tokens.get_location() );

  size_t value = string_to_size_t( tokens.eat() );

  if( value > 127 )
    throw_exception( "Value must be < 128", tokens.get_prev_location() );

  if( !tokens.match( INTEGER ) )
    throw_exception( "Expected integer time", tokens.get_location() );

  size_t time = string_to_size_t( tokens.eat() );

  check_new_time( time, tokens.get_prev_location() );

  Event event;

  event.set_ctrl( 0, ctrl_num, value, time );

  _sequence.add_event( event );
}

void TrainingSequenceParser::check_new_time( size_t time,
                                             const string& location ) {
  if( _prev_time != 0 && time < _prev_time ) {
    ostringstream oss;
    oss << "Event time " << time << " is earlier than previous time "
        << _prev_time;
    throw_exception( oss.str(), location );
  }

  _prev_time = time;
}

size_t TrainingSequenceParser::string_to_size_t( const string& str ) {
  istringstream iss( str );
  size_t val;
  iss >> val;
  return val;
}

void TrainingSequenceParser::throw_exception( const string& error,
                                              const string& location ) {
  ostringstream oss;
  oss << "Error at " << location << ": " << error;
  throw TrainingSequenceParserException( oss.str() );
}
