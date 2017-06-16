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

#include "json_importer.hpp"

using namespace littlelstm;
using namespace nlohmann;
using namespace std;

size_t JsonImporter::get_input_count() {
  try {
    return _json["arch_input_count"];
  }
  catch( const domain_error& e ) {
    throw JsonImporterException( e.what() );
  }
}

size_t JsonImporter::get_output_count() {
  try {
    return _json["arch_output_count"];
  }
  catch( const domain_error& e ) {
    throw JsonImporterException( e.what() );
  }
}

size_t JsonImporter::get_unit_count() {
  try {
    return _json["arch_unit_count"];
  }
  catch( const domain_error& e ) {
    throw JsonImporterException( e.what() );
  }
}

vector< pair<Id_t, Id_t> > JsonImporter::get_connections() {
  vector<pair<Id_t,Id_t> > connections;

  try {
    if( _json["connections"].size() % 2 != 0 ) {
      string error = "Invalid number of connections";
      throw JsonImporterException( error );
    }

    for( size_t i = 0; i < _json["connections"].size(); i += 2 ) {
      connections.emplace_back( _json["connections"][i],
                                _json["connections"][i+1] );
    }
  }
  catch( const domain_error& e ) {
    throw JsonImporterException( e.what() );
  }

  return connections;
}

const vector<LstmUnitProperties> JsonImporter::get_units_properties() {
  vector<LstmUnitProperties> properties;

  try {
    unordered_map<Id_t, vector<LstmGatedConn> > gated_conns;

    if( _json["gated_conns"].size() % 3 != 0 ) {
      string error = "Invalid gated connections";
      throw JsonImporterException( error );
    }

    for( size_t i = 0; i < _json["gated_conns"].size(); i += 3 ) {
      Id_t gater_id = _json["gated_conns"][i];
      Id_t in_id = _json["gated_conns"][i+1];
      Id_t out_id = _json["gated_conns"][i+2];

      if( gated_conns.count( gater_id ) == 0 )
        gated_conns[gater_id] = vector<LstmGatedConn>();

      gated_conns[gater_id].emplace_back( gater_id, in_id, out_id );
    }

    for( auto& unit_json : _json["units_properties"] ) {
      Id_t id = unit_json["id"];
      lstm_unit_t type = string_to_unit_type( unit_json["type"] );
      lstm_act_func_t act_func_type =
        string_to_act_func_type( unit_json["act_func"] );

      Id_t self_conn_gater;

      try {
        self_conn_gater = unit_json["self_conn_gater"];
      }
      catch ( const domain_error& e ) {
        self_conn_gater = NO_UNIT;
      }

      vector<LstmGatedConn> unit_gated_conns = gated_conns[id];

      properties.emplace_back( id, type, act_func_type, self_conn_gater,
                               unit_gated_conns );
    }
  }
  catch( const domain_error& e ) {
    throw JsonImporterException( e.what() );
  }

  return properties;
}

WeightsMap_t JsonImporter::get_weights() {
  WeightsMap_t weights_map;

  try {
    auto it = _json["weights"].begin();

    for( size_t i = 0; i < _json["connections"].size(); i += 2 ) {
      Id_t in_id = _json["connections"][i];
      Id_t out_id = _json["connections"][i + 1];

      weights_map[out_id][in_id] = *it;

      ++it;
    }
  }
  catch( const domain_error& e ) {
    throw JsonImporterException( e.what() );
  }

  return weights_map;
}
