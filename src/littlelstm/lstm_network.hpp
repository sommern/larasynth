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

#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>

#include "lstm_types.hpp"
#include "lstm_gated_connection.hpp"
#include "lstm_architecture.hpp"
#include "lstm_unit_properties.hpp"
#include "rand_gen.hpp"

namespace littlelstm {

class LstmArchitecture;

class LstmNetwork {
public:
  explicit LstmNetwork( LstmArchitecture& arch, bool training = true );
  LstmNetwork( size_t input_count,
               size_t output_count, size_t unit_count, 
               std::vector< std::pair<Id_t, Id_t> > connections,
               std::vector<LstmUnitProperties> units_properties,
               bool training = true );
  void print_weights();

  void feed_forward( const std::vector<double>& input );
  void backpropagate( const std::vector<double>& target,
                      const double learning_rate,
                      const double momentum );

  std::vector<double> get_output() { return _output; }
  std::size_t get_output_size() { return _output.size(); }
  std::size_t get_input_size() { return _input.size(); }

  std::map<Id_t, double> get_cell_states();

  WeightsMap_t get_weights_map();
  void set_weights( WeightsMap_t& weights_map );

  void zero_network();

private:
  void calculate_activations();
  void calculate_extended_eligibility_traces();
  void calculate_error_responsibilities();
  void update_weights( const double learning_rate, const double momentum );
  double uniform_random_weight( double min = -1.0, double max = 1.0 );
  double normal_random_weight( double mean = 0.0, double stddev = 0.1 );  

  std::size_t _unit_count;
  std::size_t _input_count;
  std::size_t _output_count;

  Id_t _bias_id;
  Id_t _first_output_id;

  std::vector<double> _input;
  std::vector<double> _output;
  std::vector<double> _target;

  bool _training;

  // These vectors are all indexed by unit IDs.
  std::vector<double> _old_states;
  std::vector<double> _states;
  
  std::vector<double> _activations;

  std::vector<double (*)(double)> _act_funcs;
  std::vector<double (*)(double)> _act_func_derivatives;

  std::vector< std::vector<Id_t> > _incoming_conns;
  std::vector< std::vector<double> > _conn_gains;

  std::vector<bool> _self_conn;
  std::vector<Id_t> _self_conn_gaters;
  std::vector<double> _self_conn_gains;

  std::vector< std::vector<Id_t> > _gaters;

  std::vector< std::vector<double> > _right_term_sums;

  std::vector< std::vector<double> > _traces;

  std::vector<LstmGatedConn> _gated_conns;
  std::vector< std::vector< std::vector<double> > > _ext_traces;

  std::vector< std::unordered_set<Id_t> > _projection_sets;
  std::vector< std::unordered_set<Id_t> > _gated_sets;

  std::vector<double> _error_resp_ps;
  std::vector<double> _error_resp_gs;

  std::vector<double> _error_resps;

  std::vector< std::vector<double> > _weights;
  std::vector< std::vector<double> > _old_weight_changes;

  RandGen _rand_gen;
};

}
