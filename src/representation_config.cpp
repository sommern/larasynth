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

#include "representation_config.hpp"

using namespace std;
using namespace larasynth;

RepresentationConfig::RepresentationConfig( vector<size_t>
                                            ctrl_output_counts_list,
                                            size_t update_rate,
                                            feature_config_t
                                            input_feature_config )
  : _update_rate( update_rate )
  , _input_feature_config( input_feature_config )
{
  for( size_t i = 0; i < ctrl_output_counts_list.size(); i += 2 ) {
    _ctrl_output_counts[ctrl_output_counts_list[i]] =
      ctrl_output_counts_list[i+1];
  }
}

RepresentationConfig::RepresentationConfig( ConfigParameters& config_params ) {
  vector<size_t> output_counts;
  vector<string> input_features;

  try {
    config_params.set_var( "controller_output_counts", output_counts );
    config_params.set_var( "update_rate", _update_rate, (size_t)1,
                           (size_t)100 );
    config_params.set_var( "input_features", input_features );
  }
  catch( runtime_error& e ) {
    throw RepresentationConfigException( e.what() );
  }

  if( config_params.get_unset_params().size() > 0 ) {
    ostringstream error;
    error << "The following parameters are not recognized:"
          << endl;
    for( auto& name : config_params.get_unset_params() )
      error << name << endl;
    throw RepresentationConfigException( error.str() );
  }

  if( output_counts.size() % 2 != 0 )
    throw RepresentationConfigException( "Controller output counts are set like so:\ncontroller_output_counts: <ctrl 1>, <ctrl 1 count>, <ctrl 2>, <ctrl 2 count> ..." );

  for( size_t i = 0; i < output_counts.size(); i += 2 ) {
    size_t ctrl = output_counts[i];
    size_t count = output_counts[i+1];

    if( ctrl > 127 ) {
      ostringstream error;
      error << ctrl
            << " is not a valid controller number in controller_output_counts."
            << endl << "Valid values are 0-127" << endl;
      throw RepresentationConfigException( error.str() );
    }

    if( count == 0 ) {
      ostringstream error;
      error << "Output count for controller " << (unsigned int)ctrl
            << " cannot be 0.";
      throw RepresentationConfigException( error.str() );
    }



    _ctrl_output_counts[ctrl] = count;
  }

  for( string& feature : input_features ) {
    if( feature_string_to_type.count( feature ) )
      _input_feature_config.set( feature_string_to_type.at( feature ), 1 );
    else {
      ostringstream error;
      error << "\"" << feature
            << "\" is not a valid input feature. Valid features:" << endl;
      for( auto& kv : feature_string_to_type )
        error << "  \"" << kv.first << "\"" << endl;
      throw RepresentationConfigException( error.str() );
    }
  }

  for( const string& name : config_params.get_unset_params() )
    throw RepresentationConfigException( "Unknown parameter " + name );
}

size_t RepresentationConfig::get_total_output_count() const {
  size_t sum = 0;

  for( auto& kv : _ctrl_output_counts )
    sum += kv.second;

  return sum;
}

void RepresentationConfig::print_representation_configuration() {
  cout << "Controller output counts:" << endl;

  for( auto& kv : _ctrl_output_counts )
    cout << "  " << (unsigned int)kv.first << ": " << kv.second << endl;

  cout << "Update rate: " << _update_rate << endl;

  cout << "Input features:" << endl;

  for( auto& kv : feature_string_to_type ) {
    string name = kv.first;
    input_feature_t type = kv.second;

    if( _input_feature_config[type] )
      cout << "  " << name << endl;
  }
}
