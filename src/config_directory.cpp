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

#include "config_directory.hpp"

using namespace std;
using namespace larasynth;

ConfigDirectory::ConfigDirectory( const string& directory_name )
  : _processed( false )
  , _dir_name( directory_name )
{
  if( _dir_name == "" )
    throw ConfigDirectoryException( "empty directory name" );

  append_slash_if_necessary( _dir_name );

  _config_file_path = _dir_name + "larasynth.conf";
}

void ConfigDirectory::process_directory() {
  if( !is_directory( _dir_name ) )
    throw ConfigDirectoryException( _dir_name + " is not a directory" );

  if( !is_regular_file( _config_file_path ) )
    throw ConfigDirectoryException( _config_file_path + " does not exist" );

  _results_dir_name = _dir_name + "training_results/";
  _examples_dir_name = _dir_name + "training_examples/";

  if( !is_directory( _results_dir_name ) )
    make_directory( _results_dir_name );

  if( !is_directory( _examples_dir_name ) )
    make_directory( _examples_dir_name );

  vector<string> filenames;
  vector<string> root_dir_subdirs;

  get_directory_filenames_and_subdirs( _dir_name, filenames,
                                       root_dir_subdirs );

  if( find( filenames.begin(), filenames.end(), "larasynth.conf" )
      == filenames.end() )
    throw ConfigDirectoryException( "no larasynth.conf found in " +
                                    _dir_name );

  if( find( root_dir_subdirs.begin(), root_dir_subdirs.end(),
            "training_results" ) != root_dir_subdirs.end() ) {
    vector<string> subdirs;
    string results_dir = _dir_name + "training_results/";
    get_directory_filenames_and_subdirs( results_dir, filenames, subdirs );

    for( string& filename : filenames ) {
      if( is_valid_training_results_filename( filename ) )
        _training_results_filenames.push_back( results_dir + filename );
    }

    sort( _training_results_filenames.begin(),
          _training_results_filenames.end() );
  }

  if( find( root_dir_subdirs.begin(), root_dir_subdirs.end(),
            "training_examples" ) != root_dir_subdirs.end() ) {
    vector<string> subdirs;
    string examples_dir = _dir_name + "training_examples/";

    get_directory_filenames_and_subdirs( examples_dir, filenames, subdirs );

    for( string& filename : filenames ) {
      if( is_valid_training_example_filename( filename ) )
        _training_example_filenames.push_back( examples_dir + filename );
    }

    sort( _training_example_filenames.begin(),
          _training_example_filenames.end() );
  }

  _processed = true;
}

bool ConfigDirectory::is_valid_training_example_filename( const string&
                                                          filename ) {
  //static regex example_re( "example-\\d\\d\\d\\d-\\d\\d-\\d\\d-\\d\\d:\\d\\d:\\d\\d.seq" );
  static regex example_re( ".+\\.seq" );

  smatch m;

  return regex_match( filename, m, example_re );
}

bool ConfigDirectory::is_valid_training_results_filename( const string&
                                                          filename ) {
  //static regex results_re( "results-\\d\\d\\d\\d-\\d\\d-\\d\\d-\\d\\d:\\d\\d:\\d\\d.json" );
  static regex results_re( ".+\\.json" );
  smatch m;

  return regex_match( filename, m, results_re );
}

string ConfigDirectory::get_new_training_example_filename() {
  ostringstream oss;

  oss << _examples_dir_name << "example-" << get_timestamp_string() << ".seq";

  return oss.str();
}

string ConfigDirectory::get_new_training_results_filename() {
  ostringstream oss;

  oss << _results_dir_name << "results-" << get_timestamp_string() << ".json";

  return oss.str();
}

void ConfigDirectory::write_example_config_file() {
  ofstream of( _config_file_path.c_str() );

  of << example_config;
}
