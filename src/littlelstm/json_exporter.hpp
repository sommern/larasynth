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

#include "json.hpp"
#include "network_exporter.hpp"

namespace littlelstm {

class JsonExporterException : public std::runtime_error {
public:
  explicit JsonExporterException( const std::string& message )
    : runtime_error( message ) {};
};

class JsonExporter : public NetworkExporter {
public:
  nlohmann::json get_json() { return _json; }
  std::string get_json_string() { return _json.dump( 1 ); }

  void set_input_count( size_t input_count );
  void set_output_count( size_t output_count );
  void set_unit_count( size_t unit_count );
  void set_connections( const std::vector< std::pair<Id_t, Id_t> >&
                        connections );
  void set_units_properties( const std::vector<LstmUnitProperties>&
                             units_properties );
  void set_weights( const WeightsMap_t& weights );

private:
  nlohmann::json _json;
};

}
