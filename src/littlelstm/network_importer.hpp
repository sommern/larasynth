/*
Copyright 2017 Nathan Sommer

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

#include "lstm_types.hpp"
#include "lstm_unit_properties.hpp"

namespace littlelstm {

class NetworkImporter {
public:
  virtual size_t get_input_count() =0;
  virtual size_t get_output_count() =0;
  virtual size_t get_unit_count() =0;
  virtual std::vector< std::pair<Id_t, Id_t> > get_connections() =0;
  virtual const std::vector<LstmUnitProperties> get_units_properties() =0;
  virtual WeightsMap_t get_weights() =0;
};

}
