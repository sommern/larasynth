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
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <cassert>

#include "debug.hpp"
#include "example_config_string.hpp"
#include "filesystem_operations.hpp"
#include "time_utilities.hpp"

namespace larasynth {

class ConfigDirectoryException : public std::runtime_error {
public:
  explicit ConfigDirectoryException( const std::string& message )
    : runtime_error( message ) {};
};

class ConfigDirectory {
public:
  explicit ConfigDirectory( const std::string& directory_name );

  void process_directory();

  bool exists() { return is_directory( _dir_name ); }
  bool config_file_exists() { return is_regular_file( _config_file_path ); }

  void create() { make_directory( _dir_name ); }
  void write_example_config_file();

  std::string get_directory_name() { return _dir_name; }
  std::string get_config_file_path() { return _config_file_path; }
  std::vector<std::string> get_training_example_filenames()
  { return _training_example_filenames; }
  std::vector<std::string> get_training_results_filenames()
  { return _training_results_filenames; }
  bool training_examples_exist()
  { return _training_example_filenames.size() > 0; }
  bool training_results_exist()
  { return _training_results_filenames.size() > 0; }

  std::string get_new_training_example_filename();
  std::string get_new_training_results_filename();
  
private:
  bool is_valid_training_example_filename( const std::string& filename );
  bool is_valid_training_results_filename( const std::string& filename );

  bool _processed;

  std::string _dir_name;
  std::string _config_file_path;
  std::string _examples_dir_name;
  std::vector<std::string> _training_example_filenames;
  std::string _results_dir_name;
  std::vector<std::string> _training_results_filenames;
};

}
