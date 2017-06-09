/*
Copyright 2017 Nathan Sommer

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
#include <stdexcept>

#include "config_parameters.hpp"

namespace larasynth {

class PerformingConfigException : public std::runtime_error {
public:
  PerformingConfigException( const std::string& message )
    : runtime_error( "Error in configuration section [performing]:\n"
                     + message ) {};
};

class PerformingConfig {
public:
  PerformingConfig( ConfigParameters& config_params );

  std::string get_training_results_filename()
  { return _training_results_filename; }

private:
  std::string _training_results_filename;
};

}
