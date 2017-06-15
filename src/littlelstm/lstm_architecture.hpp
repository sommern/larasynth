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

#include <vector>
#include <utility>
#include <map>
#include <cassert>

#include "lstm_unit_properties.hpp"
#include "lstm_activation_function.hpp"
#include "lstm_order.hpp"
#include "lstm_layer_config.hpp"

namespace littlelstm {

/**
 * This class builds an architecture for an LSTM network.
 */
class LstmArchitecture {
public:
  LstmArchitecture( size_t input_count, size_t output_count,
                    std::vector<size_t> block_counts,
                    std::vector<size_t> cells_per_block );

  size_t get_unit_count() const { return _unit_count; }
  size_t get_input_count() const { return _input_count; }
  size_t get_output_count() const { return _output_count; }

  const std::vector< std::pair<Id_t, Id_t> >& get_connections() const
  { return _connections; }
  const std::vector<LstmUnitProperties>& get_units_properties() const
  { return _units_properties; }
  const LstmUnitProperties& get_unit_properties( Id_t id ) const
  { return _units_properties[id]; }

private:
  Id_t add_unit( lstm_unit_t type, lstm_act_func_t act_func = IDENTITY );
  void add_gate( Id_t in_id, Id_t out_id, Id_t gate_id );
  void add_self_gate( Id_t cell_id, Id_t gate_id );
  void add_connection( Id_t in_id, Id_t out_id );

  std::vector<Id_t> get_layer_ids_by_type( Index_t h, lstm_unit_t type );

  void setup_hidden_layer_default_weight_configs();
  void setup_output_layer_default_weight_configs();
  bool connection_needed( Id_t source_layer_id, Id_t dest_layer_id,
                          lstm_unit_t source_type, lstm_unit_t dest_type );

  size_t _unit_count;
  size_t _input_count;
  size_t _output_count;

  std::vector<LstmLayerConfig> _layer_configs;
  LstmWeightConfig _weight_config;

  std::vector< std::pair<Id_t, Id_t> > _connections;
  std::vector<LstmUnitProperties> _units_properties;

  // groups of IDs for connection building
  std::vector<Id_t> _input_ids;
  std::vector< std::vector<Id_t> > _input_gate_ids;
  std::vector< std::vector<Id_t> > _forget_gate_ids;
  std::vector< std::vector<Id_t> > _output_gate_ids;
  std::vector< std::vector<Id_t> > _all_gate_ids;
  std::vector< std::vector< std::vector<Id_t> > > _block_cell_ids;
  std::vector< std::vector<Id_t> > _all_cell_ids;
  std::vector<Id_t> _output_ids;

  // units that connect to cells and units that cells connect to, for gating
  std::map< Id_t, std::vector<Id_t> > _gated_cell_inputs;
  std::map< Id_t, std::vector<Id_t> > _gated_cell_outputs;

  Id_t _next_id;
};

}
