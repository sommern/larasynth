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

#include "training_results.hpp"

using namespace std;
using namespace larasynth;
using namespace nlohmann;

TrainingResults::TrainingResults( const string& filename,
                                  training_results_mode mode )
  : _filename( filename )
  , _mode( mode )
{
  if( _mode == READ_RESULTS ) {
    if( !is_regular_file( _filename ) ) {
      string error = _filename + " does not exist or is not a regular file.";
      throw TrainingResultsException( error );
    }

    string json_data;

    try {
      ifstream infile( _filename );
      stringstream buffer;
      buffer << infile.rdbuf();
      json_data = buffer.str();
    }
    catch( ios_base::failure& e ) {
      throw TrainingResultsException( e.what() );
    }

    _json = json::parse( json_data );
  }
  else if( _mode == WRITE_RESULTS ) {
    if( is_regular_file( _filename ) ) {
      string error = _filename + " already exists.";
      throw TrainingResultsException( error );
    }
  }
}

void TrainingResults::add_weights( const WeightsMap_t& weights ) {
  if( _json.find( "connections" ) == _json.end() ) {
    string error = "connections must be added to results before weights";
    throw TrainingResultsException( error );
  }

  for( size_t i = 0; i < _json["connections"].size(); i += 2 ) {
    Id_t in_id = _json["connections"][i];
    Id_t out_id = _json["connections"][i + 1];

    _json["weights"].push_back( weights.at( out_id ).at ( in_id ) );
  }
}

WeightsMap_t TrainingResults::get_weights() {
  WeightsMap_t weights_map;

  try {
    auto it = _json["weights"].begin();

    for( size_t i = 0; i < _json["connections"].size(); i += 2 ) {
      Id_t in_id = _json["connections"][i];
      Id_t out_id = _json["connections"][i + 1];

      weights_map[out_id][in_id] = *it;

      ++it;
    }
  }
  catch( const domain_error& e ) {
    throw TrainingResultsException( e.what() );
  }

  return weights_map;
}

void TrainingResults::add_architecture( const LstmArchitecture& arch ) {
  _json["arch_unit_count"] = arch.get_unit_count();
  _json["arch_input_count"] = arch.get_input_count();
  _json["arch_output_count"] = arch.get_output_count();

  add_connections( arch.get_connections() );
  add_units_properties( arch.get_units_properties() );
}

void TrainingResults::add_connections( const vector< pair< Id_t, Id_t > >&
                                       connections ) {
  for( auto& connection : connections ) {
    Id_t in_id = connection.first;
    Id_t out_id = connection.second;

    _json["connections"].push_back( in_id );
    _json["connections"].push_back( out_id );
  }
}

vector<pair<Id_t,Id_t> > TrainingResults::get_connections() {
  vector<pair<Id_t,Id_t> > connections;

  if( _json["connections"].size() % 2 != 0 ) {
    string error = "Invalid number of connections in " + _filename;
    throw TrainingResultsException( error );
  }

  for( size_t i = 0; i < _json["connections"].size(); i += 2 ) {
    connections.emplace_back( _json["connections"][i],
                              _json["connections"][i+1] );
  }

  return connections;
}

void TrainingResults::add_units_properties( const vector<LstmUnitProperties>&
                                            units_properties ) {
  for( auto& unit_properties : units_properties ) {
    json unit_json;

    unit_json["id"] = unit_properties.get_id();
    unit_json["type"] = unit_type_to_string( unit_properties.get_type() );
    unit_json["act_func"] =
      act_func_type_to_string( unit_properties.get_act_func_type() );
    unit_json["self_conn_gater"] = unit_properties.get_self_conn_gater();

    _json["units_properties"].push_back( unit_json );

    add_gated_conns( unit_properties.get_gated_conns() );
  }
}

vector<LstmUnitProperties> TrainingResults::get_units_properties() {
  unordered_map<Id_t, vector<LstmGatedConn> > gated_conns =
    get_all_gated_conns();

  vector<LstmUnitProperties> properties;

  for( auto& unit_json : _json["units_properties"] ) {
    Id_t id = unit_json["id"];
    lstm_unit_t type = string_to_unit_type( unit_json["type"] );
    lstm_act_func_t act_func_type =
      string_to_act_func_type( unit_json["act_func"] );
    Id_t self_conn_gater = unit_json["self_conn_gater"];
    vector<LstmGatedConn> unit_gated_conns = gated_conns[id];

    properties.emplace_back( id, type, act_func_type, self_conn_gater,
                             unit_gated_conns );
  }

  return properties;
}

void TrainingResults::add_gated_conns( const vector<LstmGatedConn>& conns ) {
  for( auto& gated_conn : conns ) {
    _json["gated_conns"].push_back( gated_conn.gater_id );
    _json["gated_conns"].push_back( gated_conn.in_id );
    _json["gated_conns"].push_back( gated_conn.out_id );    
  }
}

unordered_map<Id_t, vector<LstmGatedConn> >
TrainingResults::get_all_gated_conns() {
  unordered_map<Id_t, vector<LstmGatedConn> > gated_conns;

  if( _json["gated_conns"].size() % 3 != 0 ) {
    string error = "Invalid gated connections in " + _filename;
    throw TrainingResultsException( error );
  }

  for( size_t i = 0; i < _json["gated_conns"].size(); i += 3 ) {
    Id_t gater_id = _json["gated_conns"][i];
    Id_t in_id = _json["gated_conns"][i+1];
    Id_t out_id = _json["gated_conns"][i+2];

    if( gated_conns.count( gater_id ) == 0 )
      gated_conns[gater_id] = vector<LstmGatedConn>();

    gated_conns[gater_id].emplace_back( gater_id, in_id, out_id );
  }

  return gated_conns;
}

void TrainingResults::add_min_max( const MidiMinMax& min_max ) {
  _json["note_min"] = min_max.get_note_min();
  _json["note_max"] = min_max.get_note_max();

  bool add_ctrls = false;

  if( _json.find( "ctrls" ) == _json.end() )
    add_ctrls = true;

  for( size_t ctrl = 0; ctrl < 128; ++ctrl ) {
    event_data_t min = min_max.get_ctrl_min( ctrl );
    event_data_t max = min_max.get_ctrl_max( ctrl );
    
    // min is greater than max if the min/max was never set up for this
    // controller
    if( min > max )
      continue;

    if( add_ctrls )
      _json["ctrls"].push_back( ctrl );

    _json["ctrl_min_max"].push_back( ctrl );
    _json["ctrl_min_max"].push_back( min_max.get_ctrl_min( ctrl ) );
    _json["ctrl_min_max"].push_back( min_max.get_ctrl_max( ctrl ) );
  }
}

MidiMinMax TrainingResults::get_min_max() {
  MidiMinMax min_max;

  size_t note_min = _json["note_min"];
  size_t note_max = _json["note_max"];  

  min_max.set_note_min( note_min );
  min_max.set_note_max( note_max );

  if( _json["ctrl_min_max"].size() % 3 != 0 ) {
    string error = "Invalid control min/max values in " + _filename;
    throw TrainingResultsException( error );
  }

  for( size_t i = 0; i < _json["ctrl_min_max"].size(); i += 3 ) {
    event_data_t ctrl = _json["ctrl_min_max"][i];
    event_data_t ctrl_min = _json["ctrl_min_max"][i+1];
    event_data_t ctrl_max = _json["ctrl_min_max"][i+2];    

    min_max.set_ctrl_min( ctrl, ctrl_min );
    min_max.set_ctrl_max( ctrl, ctrl_max );
  }

  return min_max;
}

LstmNetwork TrainingResults::get_trained_network() {
  size_t unit_count = _json["arch_unit_count"];
  size_t input_count = _json["arch_input_count"];
  size_t output_count = _json["arch_output_count"];  

  LstmNetwork net( input_count, output_count, unit_count,
                    get_connections(), get_units_properties(), false );

  WeightsMap_t weights = get_weights();

  net.set_weights( weights );

  return net;
}

void TrainingResults::add_result( const LstmResult& result ) {
  _json["epoch"] = result.get_epoch();
  _json["mse"] = result.get_mse();

  const vector<ctrl_values_t>& targets = result.get_targets();
  const vector<ctrl_values_t>& outputs = result.get_outputs();
  const vector< map<size_t, double> >& cell_states = result.get_cell_states();

  if( _json.find( "cell_count" ) == _json.end() )
    _json["cell_count"] = cell_states[0].size();

  if( _json.find( "ctrls" ) == _json.end() ) {
    vector<event_data_t> ctrls;
    for( auto& kv : targets[0] )
      ctrls.push_back( kv.first );
    sort( ctrls.begin(), ctrls.end() );
    for( auto ctrl : ctrls ) {
      _json["ctrls"].push_back( ctrl );
    }
  }

  _json["sample_count"] = targets.size();

  for( size_t sample_i = 0; sample_i < targets.size(); ++sample_i ) {
    for( event_data_t ctrl : _json["ctrls"] ) {
      _json["targets"].push_back( targets[sample_i].at( ctrl ) );
      _json["outputs"].push_back( outputs[sample_i].at( ctrl ) );
    }

    for( auto& kv : cell_states[sample_i] ) {
      double value = kv.second;

      _json["cell_states"].push_back( value );
    }
  }
}

void TrainingResults::add_lstm_config( const LstmConfig& lstm_config ) {
  _json["lstm_config"]["block_counts"] = lstm_config.get_block_counts();
  _json["lstm_config"]["learning_rate"] = lstm_config.get_learning_rate();
  _json["lstm_config"]["momentum"] = lstm_config.get_momentum();
}

void TrainingResults::add_repr_config( const RepresentationConfig&
                                       repr_config ) {
  vector<size_t> ctrl_output_counts_list;

  for( const auto& kv : repr_config.get_ctrl_output_counts() ) {
    ctrl_output_counts_list.push_back( kv.first );
    ctrl_output_counts_list.push_back( kv.second );    
  }

  json config_json;

  config_json["ctrl_output_counts"] = ctrl_output_counts_list;
  config_json["update_rate"] = repr_config.get_update_rate();
  config_json["use_feature_some_note_on"] =
    repr_config.use_feature_some_note_on();
  config_json["use_feature_note_struck"] =
    repr_config.use_feature_note_struck();
  config_json["use_feature_note_released"] =
    repr_config.use_feature_note_released();
  config_json["use_feature_velocity"] = repr_config.use_feature_velocity();
  config_json["use_feature_interval"] = repr_config.use_feature_interval();

  _json["representation_config"] = config_json;
}

RepresentationConfig TrainingResults::get_repr_config() {
  json config_json = _json["representation_config"];

  feature_config_t feature_config;

  if( config_json["use_feature_some_note_on"] )
    feature_config[SOME_NOTE_ON] = true;
  if( config_json["use_feature_note_struck"] )
    feature_config[NOTE_STRUCK] = true;
  if( config_json["use_feature_note_released"] )
    feature_config[NOTE_RELEASED] = true;
  if( config_json["use_feature_velocity"] )
    feature_config[VELOCITY] = true;
  if( config_json["use_feature_interval"] )
    feature_config[INTERVAL] = true;

  RepresentationConfig config( config_json["ctrl_output_counts"],
                               config_json["update_rate"],
                               feature_config );

  return config;
}

void TrainingResults::add_training_config( const TrainingConfig&
                                           training_config ) {
  json config_json;

  config_json["example_repetitions"] =
    training_config.get_example_repetitions();
  config_json["validation_example_repetitions"] =
    training_config.get_validation_example_repetitions();
  config_json["epoch_count_before_validating"] =
    training_config.get_epoch_count_before_validating();
  config_json["tempo_adjustment_factor"] =
    training_config.get_tempo_adjustment_factor();
  config_json["tempo_jitter_factor"] =
    training_config.get_tempo_jitter_factor();
  config_json["mean_padding"] = training_config.get_mean_padding();
  config_json["padding_stddev"] = training_config.get_padding_stddev();
  config_json["zero_network_before_each_epoch"] =
  training_config.get_zero_network_before_each_epoch();
  config_json["zero_network_before_validation"] =
    training_config.get_zero_network_before_validation();
  config_json["backpropagate_if_correct"] =
    training_config.get_backpropagate_if_correct();
  config_json["reset_probability"] = training_config.get_reset_probability();
  config_json["consecutive_failures_for_reset"] =
    training_config.get_consecutive_failures_for_reset();
  config_json["squared_error_failure_tolerance"] =
    training_config.get_squared_error_failure_tolerance();
  config_json["zero_network_on_reset"] =
    training_config.get_zero_network_on_reset();
  config_json["mse_threshold"] = training_config.get_mse_threshold();
  config_json["max_epoch_count"] = training_config.get_max_epoch_count();

  _json["training_config"] = config_json;
}

void TrainingResults::write() {
  if( is_regular_file( _filename ) )
    throw TrainingResultsException( _filename + " already exists." );

  try {
    ofstream outfile( _filename );
    outfile << _json.dump( 1 );
  }
  catch( const ios_base::failure& e ) {
    throw TrainingResultsException( e.what() );
  }
}
