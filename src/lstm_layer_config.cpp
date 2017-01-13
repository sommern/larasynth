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

#include "lstm_layer_config.hpp"

using namespace larasynth;

void LstmLayerConfig::add_weight_config( Id_t other_id,
                                          lstm_unit_t source_type,
                                          lstm_unit_t dest_type,
                                          LstmWeightConfig config ) {
  _weight_configs[other_id][source_type][dest_type] = config;
}

bool LstmLayerConfig::connection_needed( Id_t other_id,
                                          lstm_unit_t source_type,
                                          lstm_unit_t dest_type ) {
  if( !_weight_configs.count( other_id ) )
    return false;

  if( !_weight_configs[other_id].count( source_type ) )
    return false;

  if( !_weight_configs[other_id][source_type].count( dest_type ) )
    return false;

  return true;
}

LstmWeightConfig LstmLayerConfig::get_weight_config( Id_t other_id,
                                                       lstm_unit_t source_type,
                                                       lstm_unit_t dest_type ){
  return _weight_configs[other_id][source_type][dest_type];
}
