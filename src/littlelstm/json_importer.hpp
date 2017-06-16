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

#include <unordered_map>

#include "json.hpp"
#include "network_importer.hpp"

namespace littlelstm {

class JsonImporterException : public std::runtime_error {
public:
  explicit JsonImporterException( const std::string& message )
    : runtime_error( message ) {};
};

class JsonImporter : public NetworkImporter {
public:
  void set_json( nlohmann::json json ) { _json = json; };
  void set_json( std::string string )
  { _json = nlohmann::json::parse( string ); }

  size_t get_input_count();
  size_t get_output_count();
  size_t get_unit_count();
  std::vector< std::pair<Id_t, Id_t> > get_connections();
  const std::vector<LstmUnitProperties> get_units_properties();
  WeightsMap_t get_weights();

private:
  nlohmann::json _json;
};

}
