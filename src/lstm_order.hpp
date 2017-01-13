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

// -*-c++-*-

#pragma once

#include <vector>

#include "lstm_types.hpp"

namespace larasynth {

struct LstmOrder {
public:
  LstmOrder() {
    _hidden_layer_source_types.push_back( PREV_LAYER );
    _hidden_layer_source_types.push_back( INPUT_LAYER );
    _hidden_layer_source_types.push_back( CELL );
    _hidden_layer_source_types.push_back( INPUT_GATE );
    _hidden_layer_source_types.push_back( FORGET_GATE );
    _hidden_layer_source_types.push_back( OUTPUT_GATE );
    _hidden_layer_source_types.push_back( BIAS_UNIT );

    _hidden_layer_dest_types.push_back( CELL );
    _hidden_layer_dest_types.push_back( INPUT_GATE );
    _hidden_layer_dest_types.push_back( FORGET_GATE );
    _hidden_layer_dest_types.push_back( OUTPUT_GATE );

    _output_layer_source_types.push_back( PREV_LAYER );
    _output_layer_source_types.push_back( INPUT_LAYER );
    _output_layer_source_types.push_back( BIAS_UNIT );

    _output_layer_dest_types.push_back( OUTPUT_UNIT );
  }

  std::vector<lstm_unit_t> get_source_types( lstm_layer_t layer_type ) {
    if( layer_type == HIDDEN )
      return _hidden_layer_source_types;
    else if( layer_type == OUTPUT )
      return _output_layer_source_types;
    else
      return std::vector<lstm_unit_t>();
  }

  std::vector<lstm_unit_t> get_dest_types( lstm_layer_t layer_type ) {
    if( layer_type == HIDDEN )
      return _hidden_layer_dest_types;
    else if( layer_type == OUTPUT )
      return _output_layer_dest_types;
    else
      return std::vector<lstm_unit_t>();
  }

private:
  std::vector<lstm_unit_t> _hidden_layer_source_types;
  std::vector<lstm_unit_t> _hidden_layer_dest_types;
  std::vector<lstm_unit_t> _output_layer_source_types;
  std::vector<lstm_unit_t> _output_layer_dest_types;
};

}
