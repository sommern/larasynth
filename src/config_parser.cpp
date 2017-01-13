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

#include "config_parser.hpp"

using namespace std;
using namespace larasynth;

ConfigParser::ConfigParser( string filename )
  : _filename( filename )
{
  parse();
}

bool ConfigParser::section_exists( const string& name ) {
  return _section_parameters.find( name ) != _section_parameters.end();
}

ConfigParameters ConfigParser::get_section_params( const string& name) {
  if( _section_parameters.find( name ) == _section_parameters.end() ) {
    ConfigParameters default_params;
    return default_params;
  }

  return _section_parameters[name];
}

void ConfigParser::parse() {
  Lexer lexer( _filename );
  Tokens tokens = lexer.get_tokens();

  string current_section = "";

  while( tokens.n_tokens_remaining() != 0 ) {
    if( tokens.match( L_SQUARE ) ) {
      current_section = parse_section_name( tokens );

      if( _section_parameters[current_section].size() != 0 ) {
        throw_exception( "duplicate section " + current_section,
                         tokens.get_location() );
      }

      ConfigParameters empty_params;
      _section_parameters[current_section] = empty_params;
    }
    else if( tokens.match( ALPHANUMERIC ) ) {
      string parameter_name = tokens.eat();

      if( _section_parameters[current_section].exists( parameter_name ) ) {
        throw_exception( "duplicate parameter name " + parameter_name,
                         tokens.get_location() );
      }

      if( !tokens.match( EQUALS ) && !tokens.match( COLON ) ) {
        throw_exception( "expected '=' or ':' after " + parameter_name,
                         tokens.get_location() );
      }

      tokens.eat();

      if( tokens.n_tokens_remaining() == 0 ) {
        throw_exception( "expected value after '='", tokens.get_location() );
      }

      ConfigParameter param = parse_right_side( tokens );

      _section_parameters[current_section].add( parameter_name, param );
    }
    else {
      throw ConfigParserException( "Unexpected token: " +
                                   tokens.cur_tok_to_str() );
    }
  }
}

bool ConfigParser::real_match( Tokens& tokens ) {
  vector< vector<token_t> > matches;

  matches.emplace_back( vector<token_t>( { MINUS, DOT } ) );
  matches.emplace_back( vector<token_t>( { MINUS, INTEGER, DOT } ) );
  matches.emplace_back( vector<token_t>( { INTEGER, DOT } ) );
  matches.emplace_back( vector<token_t>( { DOT } ) );

  size_t orig_matches_size = matches.size();
  
  for( size_t i = 0; i < orig_matches_size; ++i ) {
    vector<token_t> square_prefix = { L_SQUARE };
    square_prefix.insert( square_prefix.end(), matches[i].begin(),
                          matches[i].end() );

    matches.push_back( move( square_prefix ) );

    vector<token_t> curly_prefix = { L_CURLY };
    curly_prefix.insert( curly_prefix.end(), matches[i].begin(),
                         matches[i].end() );

    matches.push_back( move( curly_prefix ) );
  }

  for( auto& match : matches ) {
    if( tokens.match( match ) )
      return true;
  }

  return false;
}

bool ConfigParser::int_match( Tokens& tokens ) {
  vector< vector<token_t> > matches;

  matches.emplace_back( vector<token_t>( { INTEGER } ) );
  matches.emplace_back( vector<token_t>( { MINUS, INTEGER } ) );

  size_t orig_matches_size = matches.size();
  
  for( size_t i = 0; i < orig_matches_size; ++i ) {
    vector<token_t> square_prefix = { L_SQUARE };
    square_prefix.insert( square_prefix.end(), matches[i].begin(),
                          matches[i].end() );

    matches.push_back( move( square_prefix ) );

    vector<token_t> curly_prefix = { L_CURLY };
    curly_prefix.insert( curly_prefix.end(), matches[i].begin(),
                         matches[i].end() );

    matches.push_back( move( curly_prefix ) );
  }

  for( auto& match : matches ) {
    if( tokens.match( match ) )
      return true;
  }

  return false;
}

string ConfigParser::parse_section_name( Tokens& tokens ) {
  if( tokens.match( L_SQUARE ) )
    tokens.eat();
  else
    throw_exception( "Expected '[' in section name", tokens.get_location() );

  string section_name;

  if( tokens.match( ALPHANUMERIC ) )
    section_name = tokens.eat();
  else
    throw_exception( "Section name must be alphanumeric at '" + tokens.eat() +
                     "'", tokens.get_location() );

  if( tokens.match( R_SQUARE ) )
    tokens.eat();
  else
    throw_exception( "Expected ']' at '" + tokens.eat() + "'",
                     tokens.get_location() );

  return section_name;
}

ConfigParameter ConfigParser::parse_right_side( Tokens& tokens ) {
  // real number or real number list
  if( real_match( tokens ) ) {
    vector<double> real_list;
    parse_real_list( real_list, tokens );

    if( real_list.size() == 1 ) {
      return ConfigParameter( real_list[0] );
    }
    else {
      return ConfigParameter( real_list );
    }
  }
  // integer or integer list
  else if( int_match( tokens ) ) {
    vector<int> int_list;
    parse_int_list( int_list, tokens );

    if( int_list.size() == 1 ) {
      return ConfigParameter( int_list[0] );
    }
    else {
      return ConfigParameter( int_list );
    }
  }
  // string or string list
  else if( tokens.match( QUOTED_STRING ) ) {
    vector<string> string_list;
    parse_string_list( string_list, tokens );

    if( string_list.size() == 1 ) {
      return ConfigParameter( string_list[0] );
    }
    else {
      return ConfigParameter( string_list );
    }
  }
  else {
    throw_exception( "error at " + tokens.eat(), tokens.get_location() );
  }
}

void ConfigParser::throw_wrong_rand_list( random_type rand_type,
                                          const string& location ) {
  switch( rand_type ) {
  case UNIFORM:
    throw_exception( "uniform random parameter must have min and max",
                     location );
  case NORMAL:
    throw_exception( "normal random parameter must have mean and standard deviation", location );
  }
}

void ConfigParser::parse_real_list( vector<double>& list, Tokens& tokens ) {
  if( tokens.match( L_SQUARE ) || tokens.match( L_CURLY ) ) {
    random_type rand_type;
    token_t closing_token_type;

    if( tokens.match( L_SQUARE ) ) {
      rand_type = UNIFORM;
      closing_token_type = R_SQUARE;
    }
    else if( tokens.match( L_CURLY ) ) {
      rand_type = NORMAL;
      closing_token_type = R_CURLY;
    }
    else {
      assert( false );
    }

    tokens.eat();

    vector<double> rand_list;

    parse_real_list( rand_list, tokens );

    if( rand_list.size() != 2 )
      throw_wrong_rand_list( rand_type, tokens.get_location() );

    switch( rand_type ) {
    case UNIFORM:
      list.push_back( _rand_gen.uniform_real( rand_list[0], rand_list[1] ) );
      break;
    case NORMAL:
      list.push_back( _rand_gen.normal( rand_list[0], rand_list[1] ) );
      break;
    }

    if( !tokens.match( closing_token_type ) )
      throw_exception( "random value not closed correctly",
                       tokens.get_location() );

    tokens.eat();
  }
  else {
    string num_str;

    if( tokens.match( MINUS ) )
      num_str += tokens.eat();

    if( tokens.match( INTEGER ) )
      num_str += tokens.eat();

    if( tokens.match( DOT ) )
      num_str += tokens.eat();
    else
      throw_exception( "Real number requires '.'", tokens.get_location() );

    if( tokens.match( INTEGER ) )
      num_str += tokens.eat();

    if( num_str == "." )
      throw_exception( "Error in real number", tokens.get_location() );

    double value;
    istringstream iss( num_str );
    iss >> value;
    list.push_back( value );
  }

  if( tokens.match( COMMA ) ) {
    tokens.eat();
    parse_real_list( list, tokens );
  }
}

void ConfigParser::parse_int_list( vector<int>& list, Tokens& tokens ) {
  if( tokens.match( L_SQUARE ) || tokens.match( L_CURLY ) ) {
    random_type rand_type;
    token_t closing_token_type;

    if( tokens.match( L_SQUARE ) ) {
      rand_type = UNIFORM;
      closing_token_type = R_SQUARE;
    }
    else if( tokens.match( L_CURLY ) ) {
      rand_type = NORMAL;
      closing_token_type = R_CURLY;
    }
    else {
      assert( false );
    }

    tokens.eat();

    vector<int> rand_list;

    parse_int_list( rand_list, tokens );

    if( rand_list.size() != 2 )
      throw_wrong_rand_list( rand_type, tokens.get_location() );

    switch( rand_type ) {
    case UNIFORM:
      list.push_back( _rand_gen.uniform_int( rand_list[0], rand_list[1] ) );
      break;
    case NORMAL:
      list.push_back( (int)_rand_gen.normal( (double)rand_list[0],
                                             (double)rand_list[1] ) );
      break;
    }

    if( !tokens.match( closing_token_type ) )
      throw_exception( "random value not closed correctly",
                       tokens.get_location() );

    tokens.eat();
  }
  else {
    string num_str;

    if( tokens.match( MINUS ) )
      num_str += tokens.eat();

    if( tokens.match( INTEGER ) )
      num_str += tokens.eat();
    else
      throw_exception( "Expected an integer at '" + tokens.eat() + "'",
                       tokens.get_location() );

    int value;
    istringstream iss( num_str );
    iss >> value;
    list.push_back( value );
  }

  if( tokens.match( COMMA ) ) {
    tokens.eat();
    parse_int_list( list, tokens );
  }
}

void ConfigParser::parse_string_list( vector<string>& list, Tokens& tokens ) {
  if( tokens.match( QUOTED_STRING ) )
    list.push_back( tokens.eat() );
  else
    throw_exception( "Expected a quoted string at '" + tokens.eat() + "'",
                     tokens.get_location() );

  if( tokens.match( COMMA ) ) {
    tokens.eat();
    parse_string_list( list, tokens );
  }
}

void ConfigParser::throw_exception( const string& error,
                                    const string& location ) {
  ostringstream oss;
  oss << "Error at " << location << ": " << error;
  throw ConfigParserException( oss.str() );
}
