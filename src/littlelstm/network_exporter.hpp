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

class NetworkExporter {
public:
  NetworkExporter() {}
  ~NetworkExporter() {}

  virtual void set_input_count( size_t input_count ) =0;
  virtual void set_output_count( size_t output_count ) =0;
  virtual void set_unit_count( size_t unit_count ) =0;
  virtual void set_connections( const std::vector< std::pair<Id_t, Id_t> >&
                                connections ) =0;
  virtual void set_units_properties( const std::vector<LstmUnitProperties>&
                                     units_properties ) =0;
  virtual void set_weights( const WeightsMap_t& weights ) =0;
};

}
