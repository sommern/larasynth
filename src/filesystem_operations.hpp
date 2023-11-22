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
#include <sstream>
#include <sys/stat.h>
#include <filesystem>

namespace larasynth {

class FilesystemException : public std::runtime_error {
public:
  explicit FilesystemException( const std::string& message )
    : runtime_error( message ) {};
};

inline void append_slash_if_necessary( std::string& dirpath ) {
  if( dirpath[dirpath.length()-1] != '/' )
    dirpath.append( "/" );
}

inline bool is_regular_file( const std::string& filename ) {
  std::filesystem::path p;
  try {
    p = filename;
  }
  catch ( int e ) { // TODO: probably change what's being caught
    return false;
  }

  if( std::filesystem::is_regular_file( p ) )
      return true;
  else
    return false;
}

inline void make_directory( const std::string& dir_name ) {
  bool ret = std::filesystem::create_directory( dir_name );

  if( !ret ) {
    std::ostringstream oss;
    oss << "Error creating directory " << dir_name;
    throw FilesystemException( oss.str() );
  }
}

inline bool is_directory( const std::string& filename ) {
  std::filesystem::path p;
  try {
    p = filename;
  }
  catch ( int e ) { // TODO: probably change what's being caught
    return false;
  }

  if( std::filesystem::is_directory( p ) )
      return true;
  else
    return false;
}

inline void get_directory_filenames_and_subdirs( std::string path,
                                                 std::vector<std::string>&
                                                 filenames,
                                                 std::vector<std::string>&
                                                 subdirs ) {
  filenames.resize( 0 );
  subdirs.resize( 0 );

  append_slash_if_necessary( path );

  for( const auto & current_file : std::filesystem::directory_iterator( path ) ) {
    if( is_regular_file( current_file ) ) {
      filenames.push_back( current_file.path().string() );
    }
    else if( is_directory( current_file ) ) {
      subdirs.push_back( current_file.path().string() );
    }
  }
}

}
