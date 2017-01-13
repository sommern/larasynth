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

#include "config_parameters.hpp"

using namespace std;
using namespace larasynth;

ConfigParameter ConfigParameters::get_param_by_name( const string& name ) {
  auto it = _params.find( name );

  if( it == _params.end() )
    throw UndefinedParameterException( name );

  return it->second;
}

bool ConfigParameters::exists( const string& name ) {
  return ( _params.find( name ) != _params.end() );
}

void ConfigParameters::add( const string& name,
                            const ConfigParameter& param ) {
  _params.emplace( name, param );
  _unset_params.insert( name );
}

void ConfigParameters::set_var( const string& name,
                                string& var ) {
  const auto& param_it = _params.find( name );

  if( param_it == _params.end() )
    throw UndefinedParameterException( name );

  ConfigParameter& param = param_it->second;

  if( !param.is_correct_type( var ) ) {
    ostringstream oss;
    oss << name << " is not of the correct type." << std::endl;
    oss << expected_type_string( typeid( string ) ) << std::endl;
    throw ConfigParameterException( oss.str() );
  }

  param.set_value( var );

  _unset_params.erase( name );
}

void ConfigParameters::set_var( const string& name,
                                vector<string>& var ) {
  const auto& param_it = _params.find( name );

  if( param_it == _params.end() )
    throw UndefinedParameterException( name );

  ConfigParameter& param = param_it->second;

  if( !param.is_correct_type( var ) ) {
    ostringstream oss;
    oss << "Elements of " << name << " are not of the correct type."
        << std::endl;
    oss << expected_type_string( typeid( string ) ) << std::endl;
    throw ConfigParameterException( oss.str() );
  }

  param.set_value( var );

  _unset_params.erase( name );
}

namespace larasynth {
  string expected_type_string( const std::type_info& actual_type ) {
    std::string expected_string = "Expected type: ";

    if( actual_type == typeid( size_t ) )
      return expected_string + "positive integer, or 0";
    if( actual_type == typeid( int ) )
      return expected_string + "integer";
    if( actual_type == typeid( double ) )
      return expected_string + "real number";
    if( actual_type == typeid( bool ) )
      return expected_string + "boolean";
    if( actual_type == typeid( unsigned char ) )
      return expected_string + "integer from 0 to 127";
    if( actual_type == typeid( string ) )
      return expected_string + "quoted string";

    return "";
  }
}
