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

#include "config_parameter.hpp"

using namespace std;
using namespace larasynth;

ConfigParameter::ConfigParameter( double value )
  : _real_value( value ),
    _type( REAL_PARAMETER )
{}

ConfigParameter::ConfigParameter( const std::vector<double>& list )
  : _real_list( list ),
    _type( REAL_LIST_PARAMETER )
{}

ConfigParameter::ConfigParameter( const std::pair<double,double>& range )
  : _real_range( range ),
    _type( REAL_RANGE_PARAMETER )
{}

ConfigParameter::ConfigParameter( int value )
  : _int_value( value ),
    _type( INT_PARAMETER )
{}

ConfigParameter::ConfigParameter( const std::vector<int>& list )
  : _int_list( list ),
    _type( INT_LIST_PARAMETER )
{}

ConfigParameter::ConfigParameter( const std::pair<int,int>& range )
  : _int_range( range ),
    _type( INT_RANGE_PARAMETER )
{}

ConfigParameter::ConfigParameter( const std::string& value )
  : _string_value( value ),
    _type( STRING_PARAMETER )
{}

ConfigParameter::ConfigParameter( const std::vector<std::string>& list )
  : _string_list( list ),
    _type( STRING_LIST_PARAMETER )
{}

double ConfigParameter::get_real_value() {
  assert( _type == REAL_PARAMETER );
  return _real_value;
}

vector<double> ConfigParameter::get_real_list() {
  assert( _type == REAL_LIST_PARAMETER );
  return _real_list;
}

pair<double,double> ConfigParameter::get_real_range() {
  assert( _type == REAL_RANGE_PARAMETER );
  return _real_range;
}

int ConfigParameter::get_int_value() {
  assert( _type == INT_PARAMETER );
  return _int_value;
}

vector<int> ConfigParameter::get_int_list() {
  assert( _type == INT_LIST_PARAMETER );
  return _int_list;
}

pair<int,int> ConfigParameter::get_int_range() {
  assert( _type == INT_RANGE_PARAMETER );
  return _int_range;
}

string ConfigParameter::get_string_value() {
  assert( _type == STRING_PARAMETER );
  return _string_value;
}

vector<string> ConfigParameter::get_string_list() {
  assert( _type == STRING_LIST_PARAMETER );
  return _string_list;
}

bool ConfigParameter::is_correct_type( double var ) {
  return ( _type == REAL_PARAMETER );
}

bool ConfigParameter::is_correct_type( const vector<double>& var ) {
  return ( _type == REAL_LIST_PARAMETER || _type == REAL_PARAMETER );
}

bool ConfigParameter::is_correct_type( const pair<double,double>& var ) {
  return ( _type == REAL_RANGE_PARAMETER );
}

bool ConfigParameter::is_correct_type( int var ) {
  return ( _type == INT_PARAMETER );
}

bool ConfigParameter::is_correct_type( const vector<int>& var ) {
  return ( _type == INT_LIST_PARAMETER || _type == INT_PARAMETER );
}

bool ConfigParameter::is_correct_type( const pair<int,int>& var ) {
  return ( _type == INT_RANGE_PARAMETER );
}

bool ConfigParameter::is_correct_type( const string& var ) {
  return ( _type == STRING_PARAMETER );
}

bool ConfigParameter::is_correct_type( const vector<string>& var ) {
  return ( _type == STRING_LIST_PARAMETER || _type == STRING_PARAMETER );
}

bool ConfigParameter::is_correct_type( bool var ) {
  if( _type == INT_PARAMETER && ( _int_value == 0 || _int_value == 1 ) )
    return true;
  else if( _type == STRING_PARAMETER && ( _string_value == "true" ||
                                          _string_value == "false" ) )
    return true;
  else
    return false;
}

bool ConfigParameter::is_correct_type( size_t var ) {
  return ( _type == INT_PARAMETER && _int_value >= 0 );
}

bool ConfigParameter::is_correct_type( unsigned char var ) {
  return ( _type == INT_PARAMETER && _int_value >= 0 && _int_value <= 127 );
}

bool ConfigParameter::is_correct_type( const vector<size_t>& var ) {
  if( _type == INT_LIST_PARAMETER ) {
    bool good_vals = true;

    for( int val : _int_list ) {
      if( val < 0 ) {
        good_vals = false;
        break;
      }
    }

    return good_vals;
  }
  else if( _type == INT_PARAMETER )
    return _int_value >= 0;
  else
    return false;
}

bool ConfigParameter::is_correct_type( const vector<unsigned char>& var ) {
  if( _type == INT_LIST_PARAMETER ) {
    bool good_vals = true;

    for( int val : _int_list ) {
      if( val < 0 || val > 127 ) {
        good_vals = false;
        break;
      }
    }

    return good_vals;
  }
  else if( _type == INT_PARAMETER )
    return _int_value >= 0 && _int_value <= 127;
  else
    return false;
}

bool ConfigParameter::is_correct_type( const pair<size_t,size_t>& var ) {
  return( _type == INT_RANGE_PARAMETER && _int_range.first >= 0 &&
          _int_range.second >= 0 );
}

void ConfigParameter::set_value( double& var ) {
  var = _real_value;
}

void ConfigParameter::set_value( std::vector<double>& var ) {
  if( _type == REAL_PARAMETER ) {
    var.resize( 1 );
    var[0] = _real_value;
  }
  else
    var = _real_list;
}

void ConfigParameter::set_value( std::pair<double,double>& var ) {
  var = _real_range;
}

void ConfigParameter::set_value( int& var ) {
  var = _int_value;
}

void ConfigParameter::set_value( std::vector<int>& var ) {
  var = _int_list;
}

void ConfigParameter::set_value( std::pair<int,int>& var ) {
  var = _int_range;
}

void ConfigParameter::set_value( std::string& var ) {
  var = _string_value;
}

void ConfigParameter::set_value( std::vector<std::string>& var ) {
  if( _type == STRING_PARAMETER ) {
    var.resize( 1 );
    var[0] = _string_value;
  }
  else
    var = _string_list;
}

void ConfigParameter::set_value( bool& var ) {
  var = (bool)_int_value;
}

void ConfigParameter::set_value( size_t& var ) {
  var = (size_t)_int_value;
}

void ConfigParameter::set_value( unsigned char& var ) {
  var = (unsigned char)_int_value;
}

void ConfigParameter::set_value( std::vector<size_t>& var ) {
  if( _type == INT_PARAMETER ) {
    var.resize( 1 );
    var[0] = (size_t)_int_value;
  }
  else {
    var.resize( _int_list.size() );

    for( size_t i = 0; i < _int_list.size(); ++i )
      var[i] = (size_t)_int_list[i];
  }
}
  
void ConfigParameter::set_value( std::vector<unsigned char>& var ) {
  if( _type == INT_PARAMETER ) {
    var.resize( 1 );
    var[0] = (unsigned char)_int_value;
  }
  else {
    var.resize( _int_list.size() );

    for( size_t i = 0; i < _int_list.size(); ++i )
      var[i] = (unsigned char)_int_list[i];
  }
}
  
void ConfigParameter::set_value( std::pair<size_t,size_t>& var ) {
  var.first = (size_t)_int_range.first;
  var.second = (size_t)_int_range.second;
}

string ConfigParameter::get_type_str() {
  switch( _type ) {
  case REAL_PARAMETER:
    return "real number";
  case REAL_LIST_PARAMETER:
    return "real number list";
  case REAL_RANGE_PARAMETER:
    return "real number range";
  case INT_PARAMETER:
    return "integer";
  case INT_LIST_PARAMETER:
    return "integer list";
  case INT_RANGE_PARAMETER:
    return "integer range";
  case STRING_PARAMETER:
    return "string";
  case STRING_LIST_PARAMETER:
    return "string list";
  default:
    assert( false );
  }
}
