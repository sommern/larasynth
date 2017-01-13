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

#include <map>

#include "lstm_types.hpp"
#include "lstm_weight_generator.hpp"

namespace larasynth {

class LstmLayerConfig {
public:
  bool connection_needed( Id_t other_id, lstm_unit_t source_type,
                          lstm_unit_t dest_type );
  LstmWeightConfig get_weight_config( Id_t other_id, lstm_unit_t source_type,
                                       lstm_unit_t dest_type );

  void add_weight_config( Id_t other_id, lstm_unit_t source_type,
                          lstm_unit_t dest_type, LstmWeightConfig config );

private:
  std::map< Id_t, std::map< lstm_unit_t, std::map< lstm_unit_t, LstmWeightConfig > > >  _weight_configs;
};

}
