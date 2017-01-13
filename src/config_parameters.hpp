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
#include <map>
#include <set>
#include <limits>
#include <sstream>
#include <typeinfo>

#include "config_parameter.hpp"

namespace larasynth {

class UndefinedParameterException : public std::runtime_error {
public:
  explicit UndefinedParameterException( const std::string& name )
    : runtime_error( name + " is not defined" ) {}
};

std::string expected_type_string( const std::type_info& actual_type );

class ConfigParameters {
public:
  size_t size() { return _params.size(); }
  bool exists( const std::string& name );

  void add( const std::string& name, const ConfigParameter& param );

  // string types
  void set_var( const std::string& name, std::string& var );
  void set_var( const std::string& name, std::vector<std::string>& var ); 

  // numeric lists
  template <typename T>
  void set_var( const std::string& name, std::vector<T>& var,
                T min = std::numeric_limits<T>::lowest(),
                T max = std::numeric_limits<T>::max() );

  // numeric types
  template <typename T>
  void set_var( const std::string& name, T& var,
                T min = std::numeric_limits<T>::lowest(),
                T max = std::numeric_limits<T>::max() );

  std::set<std::string> get_unset_params() { return _unset_params; }

  ConfigParameter get_param_by_name( const std::string& name );

private:
  std::map<std::string,ConfigParameter> _params;
  std::set<std::string> _unset_params;
};

template <class T>
void ConfigParameters::set_var( const std::string& name, std::vector<T>& var,
                                T min, T max ) {
  const auto& param_it = _params.find( name );

  if( param_it == _params.end() )
    throw UndefinedParameterException( name );

  ConfigParameter& param = param_it->second;

  if( !param.is_correct_type( var ) ) {
    std::ostringstream oss;
    oss << "Elements of " << name << " are not of the correct type."
        << std::endl;
    oss << expected_type_string( typeid( T ) ) << std::endl;
    throw ConfigParameterException( oss.str() );
  }

  param.set_value( var );

  _unset_params.erase( name );

  for( auto& val : var ) {
    if( val < min ) {
      std::ostringstream oss;
      oss << "all elements of " << name << " must be at least " << min << ". "
          << val << " is out of range";
      throw ConfigParameterException( oss.str() );
    }

    if( val > max ) {
      std::ostringstream oss;
      oss << "all elements of " << name << " must be at most " << max << ". "
          << val << " is out of range";
      throw ConfigParameterException( oss.str() );
    }
  }
}

template <class T>
void ConfigParameters::set_var( const std::string& name, T& var, T min,
                                T max ) {
  const auto& param_it = _params.find( name );

  if( param_it == _params.end() )
    throw UndefinedParameterException( name );

  ConfigParameter& param = param_it->second;

  if( !param.is_correct_type( var ) ) {
    std::ostringstream oss;
    oss << name << " is not of the correct type." << std::endl;
    oss << expected_type_string( typeid( T ) ) << std::endl;
    throw ConfigParameterException( oss.str() );
  }

  param.set_value( var );

  _unset_params.erase( name );

  if( var < min ) {
    std::ostringstream oss;
    oss << name << " must be at least " << min << ". " << var
        << " is out of range";
    throw ConfigParameterException( oss.str() );
  }

  if( var > max ) {
    std::ostringstream oss;
    oss << name << " must be at most " << max << ". " << var
        << " is out of range";
    throw ConfigParameterException( oss.str() );
  }
}

}
