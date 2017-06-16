/*
Copyright 2016, 2017 Nathan Sommer

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

// The algorithm implemented here is based on Derek Monner's LSTM-g algorithm.
// D. Monner and J. A. Reggia (2012). A generalized LSTM-like training
// algorithm for second-order recurrent neural networks. Neural Networks 25,
// pp70-83.
// http://www.overcomplete.net/papers/nn2012.pdf

#include "lstm_network.hpp"

using namespace std;
using namespace littlelstm;

LstmNetwork::LstmNetwork( LstmArchitecture& arch, bool training )
  : LstmNetwork( arch.get_input_count(),
                 arch.get_output_count(), arch.get_unit_count(),
                 arch.get_connections(),
                 arch.get_units_properties(), training )
{}

LstmNetwork::LstmNetwork( NetworkImporter& importer, bool training )
  : LstmNetwork( importer.get_input_count(),
                 importer.get_output_count(),
                 importer.get_unit_count(),
                 importer.get_connections(),
                 importer.get_units_properties(),
                 training )
{
  set_weights( importer.get_weights() );
}

LstmNetwork::LstmNetwork( size_t input_count,
                          size_t output_count, size_t unit_count,
                          vector< pair<Id_t, Id_t> > connections,
                          vector<LstmUnitProperties> units_properties,
                          bool training )
  : _unit_count( unit_count )
  , _input_count( input_count )
  , _output_count( output_count )
  , _bias_id( _input_count )
  , _first_output_id( _unit_count - _output_count )
  , _input( _input_count, 0.0 )
  , _output( _output_count, 0.0 )
  , _target( _output_count, 0.0 )
  , _training( training )
  , _old_states( _unit_count, 0.0 )
  , _states( _unit_count, 0.0 )
  , _activations( _unit_count, 0.0 )
  , _act_funcs( _unit_count, nullptr )
  , _act_func_derivatives( _unit_count, nullptr )
  , _incoming_conns( _unit_count )
  , _conn_gains( _unit_count, vector<double>( _unit_count, 0.0 ) )
  , _self_conn( _unit_count, false )
  , _self_conn_gaters( _unit_count, NO_UNIT )
  , _self_conn_gains( _unit_count, 0.0 )
  , _gaters( _unit_count, vector<Id_t>( _unit_count, NO_UNIT ) )
  , _right_term_sums( _unit_count, vector<double>( _unit_count, 0.0 ) )
  , _traces( _unit_count, vector<double>( _unit_count, 0.0 ) )
  , _ext_traces( _unit_count,
                 vector< vector<double> >( _unit_count,
                                           vector<double>( _unit_count,
                                                           0.0 ) ) )
  , _projection_sets( _unit_count )
  , _gated_sets( _unit_count )
  , _error_resp_ps( _unit_count, 0.0 )
  , _error_resp_gs( _unit_count, 0.0 )
  , _error_resps( _unit_count, 0.0 )
  , _weights( _unit_count, vector<double>( _unit_count, 0.0 ) )
  , _old_weight_changes( _unit_count, vector<double>( _unit_count, 0.0 ) )
  , _units_properties( units_properties )
{
  for( auto& conn : connections ) {
    Index_t in_id = conn.first;
    Index_t out_id = conn.second;

    _incoming_conns[out_id].push_back( in_id );

    if( in_id < out_id )
      _projection_sets[in_id].insert( out_id );

    if( units_properties[in_id].get_type() == BIAS_UNIT ) {
      if( units_properties[out_id].get_type() == FORGET_GATE )
        _weights[out_id][in_id] = uniform_random_weight( 0.0, 2.0 );
      else if( units_properties[out_id].get_type() == INPUT_GATE )
        _weights[out_id][in_id] = uniform_random_weight( -2.0, 0.0 );
      else if( units_properties[out_id].get_type() == OUTPUT_GATE )
        _weights[out_id][in_id] = uniform_random_weight( -2.0, 0.0 );
      else
        _weights[out_id][in_id] = normal_random_weight( 0.0, 0.1 );
    }
    else
      _weights[out_id][in_id] = normal_random_weight( 0.0, 0.1 );
  }

  for( auto& unit_properties : units_properties ) {
    lstm_unit_t unit_type = unit_properties.get_type();
    
    if( unit_type == INPUT_UNIT || unit_type == BIAS_UNIT )
      continue;

    Id_t id = unit_properties.get_id();

    _act_funcs[id] = unit_properties.get_act_func();
    _act_func_derivatives[id] = unit_properties.get_act_func_derivative();

    if( unit_properties.get_self_conn() ) {
      _self_conn[id] = true;
      _self_conn_gaters[id] = unit_properties.get_self_conn_gater();
    }

    for( auto& conn : unit_properties.get_gated_conns() ) {
      assert( id == conn.gater_id );

      Index_t in_id = conn.in_id;
      Index_t out_id = conn.out_id;

      _gaters[out_id][in_id] = id;
      _gated_conns.push_back( conn );

      if( id < out_id )
        _gated_sets[id].insert( out_id );
    }
  }

  _activations[_bias_id] = 1.0;
}

void LstmNetwork::export_network( NetworkExporter& exporter ) const {
  exporter.set_input_count( _input_count );
  exporter.set_output_count( _output_count );
  exporter.set_unit_count( _unit_count );
  exporter.set_connections( get_connections() );
  exporter.set_units_properties( _units_properties );
  exporter.set_weights( get_weights_map() );
}

void LstmNetwork::feed_forward( const vector<double>& input ) {
  assert( input.size() == _input_count );

  //clock_t begin = clock();

  _input = input;

  calculate_activations();

  //clock_t end = clock();

  //double elapsed = end - begin;

  //cout << "fed forward in " << elapsed << endl;
}

void LstmNetwork::backpropagate( const vector<double>& target,
                                  const double learning_rate,
                                  const double momentum ) {
  //clock_t begin = clock();

  _target = target;

  calculate_extended_eligibility_traces();
  calculate_error_responsibilities();
  update_weights( learning_rate, momentum );

  //clock_t end = clock();

  //double elapsed = end - begin;

  //  cout << "backpropagated in " << elapsed << endl;
}

double LstmNetwork::uniform_random_weight( double min, double max ) {
  return _rand_gen.uniform_real( min, max );
}

double LstmNetwork::normal_random_weight( double mean, double stddev ) {
  return _rand_gen.normal( mean, stddev );
}

void LstmNetwork::zero_network() {
  fill( _states.begin(), _states.end(), 0.0 );
  fill( _activations.begin(), _activations.end(), 0.0 );
  for( Index_t i = 0; i < _old_weight_changes.size(); ++i ) {
    fill( _old_weight_changes[i].begin(), _old_weight_changes[i].end(),
          0.0 );
  }

  // return bias activation to 1.0
  _activations[_bias_id] = 1.0;
}

map<Id_t, double> LstmNetwork::get_cell_states() {
  map<Id_t, double> cell_states;

  Id_t first_id = _bias_id + 1;

  for( Id_t id = first_id; id < _unit_count; ++id ) {
    if( _self_conn[id] )
      cell_states[id] = _states[id];
  }

  return cell_states;
}


void LstmNetwork::calculate_activations() {
  copy( _input.begin(), _input.end(), _activations.begin() );

  _states.swap( _old_states );

  for( size_t i = 0; i < _right_term_sums.size(); ++i )
    fill( _right_term_sums[i].begin(), _right_term_sums[i].end(), 0.0 );

  Id_t first_id = _bias_id + 1;

  for( Id_t id = first_id; id < _unit_count; ++id ) {
    double sum = 0.0;

    if( _self_conn[id] ) {
      Id_t gater_id = _self_conn_gaters[id];

      double gain;

      if( gater_id == NO_UNIT )
        gain = 1.0;
      else
        gain = _activations[gater_id];

      sum += gain * _old_states[id];

      if( _training )
        _self_conn_gains[id] = gain;
    }
    
    for( Index_t in_i = 0; in_i < _incoming_conns[id].size(); ++in_i ) {
      Id_t in_id = _incoming_conns[id][in_i];
      Id_t gater_id = _gaters[id][in_id];

      double gain;

      if( gater_id == NO_UNIT )
        gain = 1.0;
      else {
        gain = _activations[gater_id];
        if( _training ) {
          _right_term_sums[gater_id][id] += _weights[id][in_id] *
            _activations[in_id];
          _conn_gains[id][in_id] = gain;
        }
      }

      sum += gain * _weights[id][in_id] * _activations[in_id];

      if( !_training )
        continue;

      if( _self_conn[id] )
        _traces[id][in_id] = _self_conn_gains[id] * _traces[id][in_id];
      else
        _traces[id][in_id] = 0.0;

      _traces[id][in_id] += gain * _activations[in_id];
    }

    _states[id] = sum;

    _activations[id] = _act_funcs[id]( _states[id] );
  }

  copy( _activations.begin() + _first_output_id, _activations.end(),
        _output.begin() );
}

void LstmNetwork::calculate_extended_eligibility_traces() {
  for( Id_t j = _bias_id; j < _gated_sets.size(); ++j ) {
    if( _gated_sets[j].size() == 0 )
      continue;

    for( auto& k : _gated_sets[j] ) {
      for( auto& i : _incoming_conns[j] ) {
        double right_term = 0.0;
        double trace = 0.0;

        if( _self_conn[k] ) {
          if( _self_conn_gaters[k] == j )
            right_term += _old_states[k];
          trace += _self_conn_gains[k] * _ext_traces[k][j][i];
        }

        right_term += _right_term_sums[j][k];

        trace += _act_func_derivatives[j]( _activations[j] ) * _traces[j][i]
          * right_term;

        _ext_traces[k][j][i] = trace;
      }
    }
  }
}

void LstmNetwork::calculate_error_responsibilities() {
  // calculate output unit responsibilities from target first
  for( Index_t i = 0; i < _output_count; ++i ) {
    Id_t out_id = i + _first_output_id;
    _error_resps[out_id] = _target[i] - _activations[out_id];
  }

  for( Id_t id = _first_output_id - 1; id > _bias_id; --id ) {
    double derivative = _act_func_derivatives[id]( _activations[id] );
    double sum = 0.0;

    for( auto& proj_id : _projection_sets[id] ) {
      Id_t gater_id = _gaters[proj_id][id];
      double gain;
      if( gater_id == NO_UNIT )
        gain = 1.0;
      else
        gain = _conn_gains[proj_id][id];

      sum += _error_resps[proj_id] * gain * _weights[proj_id][id];
    }

    _error_resp_ps[id] = derivative * sum;

    sum = 0.0;
    for( auto& gated_id : _gated_sets[id] ) {
      double inner_term = 0.0;
      if( _self_conn[gated_id] && _self_conn_gaters[gated_id] == id )
        inner_term += _old_states[gated_id];
      inner_term += _right_term_sums[id][gated_id];
      sum += _error_resps[gated_id] * inner_term;
    }

    _error_resp_gs[id] = derivative * sum;

    _error_resps[id] = _error_resp_ps[id] + _error_resp_gs[id];
  }
}

void LstmNetwork::update_weights( const double learning_rate,
                                   const double momentum ) {
  Id_t first_id = _bias_id + 1;
  for( Id_t id = first_id; id < _unit_count; ++id ) {
    for( Index_t in_i = 0; in_i < _incoming_conns[id].size(); ++in_i ) {
      Id_t in_id = _incoming_conns[id][in_i];

      double sum = 0.0;
      double weight_change;

      if( id >= _first_output_id ) {
        weight_change = _act_func_derivatives[id]( _activations[id] ) *
          _error_resps[id] * _activations[in_id];
        if( _gaters[id][in_id] != NO_UNIT )
          weight_change *= _conn_gains[id][in_id];
      }
      else {
        for( auto& k : _gated_sets[id] ) {
          sum += _error_resps[k] * _ext_traces[k][id][in_id];
        }

        weight_change = learning_rate * ( _error_resp_ps[id] *
                                          _traces[id][in_id] + sum );

        weight_change += momentum * _old_weight_changes[id][in_id];
      }
      
      _weights[id][in_id] += weight_change;
      _old_weight_changes[id][in_id] = weight_change;
    }
  }
}      

void LstmNetwork::set_weights( const WeightsMap_t& weights_map ) {
  for( auto& kv_out : weights_map ) {
    Id_t out_id = kv_out.first;
    for( auto& kv_in : weights_map.at( out_id ) ) {
      Id_t in_id = kv_in.first;
      double weight = kv_in.second;

      _weights[out_id][in_id] = weight;
    }
  }
}

WeightsMap_t LstmNetwork::get_weights_map() const {
  WeightsMap_t weights_map;

  for( Id_t out_id = 0; out_id < _incoming_conns.size(); ++out_id ) {
    for( Index_t i = 0; i < _incoming_conns[out_id].size(); ++i ) {
      Id_t in_id = _incoming_conns[out_id][i];
      
      weights_map[out_id][in_id] = _weights[out_id][in_id];
    }
  }

  return weights_map;
}

vector< pair<Id_t, Id_t> > LstmNetwork::get_connections() const {
  vector< pair<Id_t, Id_t> > connections;

  for( Id_t out_id = 0; out_id < _incoming_conns.size(); ++out_id ) {
    for( Id_t in_id : _incoming_conns[out_id] ) {
      connections.emplace_back( in_id, out_id );
    }
  }

  return connections;
}

void LstmNetwork::print_weights() {
  cout << "Weights:" << endl;
  for( Id_t out_id = 0; out_id < _incoming_conns.size(); ++out_id ) {
    for( size_t i = 0; i < _incoming_conns[out_id].size(); ++i ) {
      Id_t in_id = _incoming_conns[out_id][i];

      cout << " " << in_id << " -> " << out_id << ": "
           << _weights[out_id][in_id] << endl;
    }
  }
}
