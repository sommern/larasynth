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
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#include "config_parser.hpp"
#include "lstm_layer_config.hpp"
#include "rand_gen.hpp"
#include "lstm_types.hpp"
#include "debug.hpp"
#include "config_parameter.hpp"
#include "config_parameters.hpp"
#include "lstm_defaults.hpp"

namespace larasynth {

class LstmConfigException : public std::runtime_error {
public:
  LstmConfigException( const std::string& message )
    : std::runtime_error( message ) {};
};

class LstmConfig {
public:
  LstmConfig( ConfigParameters& params );

  void set_input_count( size_t input_count ) { _input_count = input_count; }
  void set_output_count( size_t output_count ) { _output_count = output_count; }

  void print_network_configuration();

  size_t get_input_count() { return _input_count; }
  size_t get_output_count() { return _output_count; }
  size_t get_hidden_layer_count() { return _block_counts.size(); }
  const std::vector<size_t>& get_block_counts() const { return _block_counts; }
  const std::vector<size_t>& get_cells_per_block() const { return _cells_per_block; }
  bool connection_needed( Id_t source_layer_id, Id_t dest_layer_id,
                          lstm_unit_t source_type, lstm_unit_t dest_type );

  LstmWeightConfig get_weight_config( size_t layer_i,
                                       lstm_unit_t source_type,
                                       lstm_unit_t dest_type );
  double get_learning_rate() const { return _learning_rate; }
  double get_momentum() const { return _momentum; }

private:
  void setup_output_layer_default_weight_configs();
  void setup_hidden_layer_default_weight_configs();
  void add_layer_to_weight_configs();
  lstm_unit_t string_to_unit_t( const std::string& s );

  size_t _input_count;
  size_t _output_count;

  std::vector<LstmLayerConfig> _layer_configs;

  LstmWeightConfig _weight_config;

  std::vector<size_t> _block_counts;
  std::vector<size_t> _cells_per_block;

  lstm_unit_t _source_type;
  Id_t _source_layer_id;
  lstm_unit_t _dest_type;
  Id_t _dest_layer_id;

  double _learning_rate;
  double _momentum;
};

}
