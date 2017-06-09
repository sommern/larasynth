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

#include "performing_config.hpp"

using namespace std;
using namespace larasynth;

PerformingConfig::PerformingConfig( ConfigParameters& config_params )
  : _training_results_filename( "" )
{
  try {
    config_params.set_var( "training_results", _training_results_filename );
  }
  catch( UndefinedParameterException& e ) {
  }
  catch( ConfigParameterException& e ) {
    throw PerformingConfigException( e.what() );
  }

  for( const string& name : config_params.get_unset_params() )
    throw PerformingConfigException( "Unknown parameter " + name );
}
