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
#include <cassert>
#include <map>

namespace larasynth {

enum config_parameter_t {
  REAL_PARAMETER,
  REAL_LIST_PARAMETER,
  REAL_RANGE_PARAMETER,
  INT_PARAMETER,
  INT_LIST_PARAMETER,
  INT_RANGE_PARAMETER,
  STRING_PARAMETER,
  STRING_LIST_PARAMETER
};

class ConfigParameterException : public std::runtime_error {
public:
  ConfigParameterException( const std::string& message )
    : runtime_error( message ) {};
};

class ConfigParameter {
public:
  ConfigParameter( double value );
  ConfigParameter( const std::vector<double>& list );
  ConfigParameter( const std::pair<double,double>& range );

  ConfigParameter( int value );
  ConfigParameter( const std::vector<int>& list );
  ConfigParameter( const std::pair<int,int>& range );

  ConfigParameter( const std::string& value );
  ConfigParameter( const std::vector<std::string>& list );

  config_parameter_t get_type() const { return _type; }
  std::string get_type_str();

  bool is_correct_type( double var );
  bool is_correct_type( const std::vector<double>& var );
  bool is_correct_type( const std::pair<double,double>& var );
  bool is_correct_type( int var );
  bool is_correct_type( const std::vector<int>& var );
  bool is_correct_type( const std::pair<int,int>& var );
  bool is_correct_type( const std::string& var );
  bool is_correct_type( const std::vector<std::string>& var );
  bool is_correct_type( bool var );
  bool is_correct_type( size_t var );
  bool is_correct_type( unsigned char var );  
  bool is_correct_type( const std::vector<size_t>& var );
  bool is_correct_type( const std::vector<unsigned char>& var );  
  bool is_correct_type( const std::pair<size_t,size_t>& var );

  void set_value( double& var );
  void set_value( std::vector<double>& var );
  void set_value( std::pair<double,double>& var );
  void set_value( int& var );
  void set_value( std::vector<int>& var );
  void set_value( std::pair<int,int>& var );
  void set_value( std::string& var );
  void set_value( std::vector<std::string>& var );
  void set_value( bool& var );
  void set_value( size_t& var );
  void set_value( unsigned char& var );  
  void set_value( std::vector<size_t>& var );
  void set_value( std::vector<unsigned char>& var );  
  void set_value( std::pair<size_t,size_t>& var );

  double get_real_value();
  std::vector<double> get_real_list();
  std::pair<double,double> get_real_range();

  int get_int_value();
  std::vector<int> get_int_list();
  std::pair<int,int> get_int_range();

  bool get_bool_value();

  std::string get_string_value();
  std::vector<std::string> get_string_list();

private:
  double _real_value;
  std::vector<double> _real_list;
  std::pair<double,double> _real_range;

  int _int_value;
  std::vector<int> _int_list;
  std::pair<int,int> _int_range;

  std::string _string_value;
  std::vector<std::string> _string_list;

  config_parameter_t _type;
};

}
