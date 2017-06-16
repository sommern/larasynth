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

#include "lstm_config.hpp"

using namespace std;
using namespace larasynth;
using namespace littlelstm;

LstmConfig::LstmConfig( ConfigParameters& params ) {
  unordered_map<string,pair<double*,double> > optional_doubles = {
    { "learning_rate", { &_learning_rate, DEFAULT_LEARNING_RATE } },
    { "momentum", { &_momentum, DEFAULT_MOMENTUM } }
  };

  for( auto& kv : optional_doubles ) {
    try {
      params.set_var( kv.first, *kv.second.first );
    }
    catch( ConfigParameterException& e ) {
      throw LstmConfigException( e.what() );
    }
    catch( UndefinedParameterException& e ) {
      *kv.second.first = kv.second.second;
    }
  }

  try {
    params.set_var( "block_counts", _block_counts );
  }
  catch( UndefinedParameterException& e ) {
    _block_counts = DEFAULT_BLOCK_COUNTS;
  }
  catch( ConfigParameterException& e ) {
    throw LstmConfigException( e.what() );
  }

  // any unset parameters are invalid
  set<string> unset_params = params.get_unset_params();
  if( unset_params.size() != 0 ) {
    string unset_param_list;
    for( auto& param : unset_params )
      unset_param_list += param + " ";

    throw LstmConfigException( "Invalid parameter(s): " + unset_param_list );
  }

  // 1 cell per block
  _cells_per_block.resize( _block_counts.size(), 1 );

  // dummy layer configs for input and bias layers
  _layer_configs.push_back( LstmLayerConfig() );
  _layer_configs.push_back( LstmLayerConfig() );

  for( size_t i = 0; i < _block_counts.size(); ++i )
    setup_hidden_layer_default_weight_configs();

  setup_output_layer_default_weight_configs();
}

void LstmConfig::print_network_configuration() {
  cout << "Block counts:";
  for( auto& block_count : _block_counts )
    cout << " " << block_count;
  cout << endl;
  cout << "Learning rate: " << _learning_rate << endl;
  cout << "Momentum:      " << _momentum << endl;
}

lstm_unit_t LstmConfig::string_to_unit_t( const string& s ) {
  if( s == "input" )
    return INPUT_LAYER;
  else if( s == "previous_layer" )
    return PREV_LAYER;
  else if( s == "input_gate" )
    return INPUT_GATE;
  else if( s == "forget_gate" )
    return FORGET_GATE;
  else if( s == "output_gate" )
    return OUTPUT_GATE;
  else if( s == "cell" )
    return CELL;
  else if( s == "bias" )
    return BIAS_UNIT;
  else if( s == "output" )
    return OUTPUT_UNIT;
  //  disabled for now
  //  else if( s == "all" )
  //    return ALL_UNITS;
  else
    return UNIT_ERROR;
}

void LstmConfig::setup_hidden_layer_default_weight_configs() {
  LstmLayerConfig layer_config;

  Id_t input_layer_id = 0;
  Id_t bias_layer_id = 1;
  Id_t this_layer_id = _layer_configs.size();

  double standard_min = -0.5;
  double standard_max = 0.5;

  LstmWeightConfig standard_range_config( UNIFORM_WEIGHT_GEN, standard_min,
                                           standard_max );

  double in_and_out_gate_bias_min = -2.0;
  double in_and_out_gate_bias_max = 0.0;

  double forget_gate_bias_min = 0.0;
  double forget_gate_bias_max = 2.0;

  vector<lstm_unit_t> hidden_layer_unit_types = { INPUT_GATE, FORGET_GATE,
                                                   OUTPUT_GATE, CELL };

  // input layer to cells and gates
  for( auto dest_type : hidden_layer_unit_types ) {
    layer_config.add_weight_config( input_layer_id, INPUT_UNIT, dest_type,
                                    standard_range_config );
  }

  // previous hidden layer cells to cells and gates
  if( this_layer_id > HIDDEN_LAYER_ID_OFFSET ) {
    for( auto dest_type : hidden_layer_unit_types ) {
      layer_config.add_weight_config( this_layer_id - 1, CELL, dest_type,
                                      standard_range_config );
    }
  }

  // inter layer connections
  for( auto source_type : hidden_layer_unit_types ) {
    for( auto dest_type : hidden_layer_unit_types ) {
      layer_config.add_weight_config( this_layer_id, source_type, dest_type,
                                      standard_range_config );
    }
  }

  // bias connections
  layer_config.add_weight_config( bias_layer_id, BIAS_UNIT, CELL,
                                  standard_range_config );
  layer_config.add_weight_config( bias_layer_id, BIAS_UNIT, INPUT_GATE,
                                  LstmWeightConfig( UNIFORM_WEIGHT_GEN,
                                                     in_and_out_gate_bias_min,
                                                     in_and_out_gate_bias_max ) );
  layer_config.add_weight_config( bias_layer_id, BIAS_UNIT, OUTPUT_GATE,
                                  LstmWeightConfig( UNIFORM_WEIGHT_GEN,
                                                     in_and_out_gate_bias_min,
                                                     in_and_out_gate_bias_max ) );
  layer_config.add_weight_config( bias_layer_id, BIAS_UNIT, FORGET_GATE,
                                  LstmWeightConfig( UNIFORM_WEIGHT_GEN,
                                                     forget_gate_bias_min,
                                                     forget_gate_bias_max ) );

  _layer_configs.push_back( layer_config );
}

void LstmConfig::setup_output_layer_default_weight_configs() {
  LstmLayerConfig layer_config;

  Id_t this_layer_id = _layer_configs.size();

  double min = -0.2;
  double max = 0.2;

  layer_config.add_weight_config( this_layer_id - 1, CELL, OUTPUT_UNIT,
                                  LstmWeightConfig( UNIFORM_WEIGHT_GEN, min,
                                                     max ) );
  layer_config.add_weight_config( 1, BIAS_UNIT, OUTPUT_UNIT,
                                  LstmWeightConfig( UNIFORM_WEIGHT_GEN, min,
                                                     max ) );

  _layer_configs.push_back( layer_config );
}

bool LstmConfig::connection_needed( Id_t source_layer_id, Id_t dest_layer_id,
                                     lstm_unit_t source_type,
                                     lstm_unit_t dest_type ) {
  return _layer_configs[dest_layer_id].connection_needed( source_layer_id,
                                                          source_type,
                                                          dest_type );
}

// LstmWeightConfig LstmConfig::get_weight_config( size_t layer_i,
//                                                  lstm_unit_t source_type,
//                                                  lstm_unit_t dest_type ) {
//   return _weight_configs[layer_i][source_type][dest_type];
// }
