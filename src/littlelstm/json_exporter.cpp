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

#include "json_exporter.hpp"

using namespace littlelstm;
using namespace nlohmann;
using namespace std;

void JsonExporter::set_input_count( size_t input_count ) {
  _json["arch_input_count"] = input_count;
}

void JsonExporter::set_output_count( size_t output_count ) {
  _json["arch_output_count"] = output_count;
}

void JsonExporter::set_unit_count( size_t unit_count ) {
  _json["arch_unit_count"] = unit_count;
}

void JsonExporter::set_connections( const std::vector< std::pair<Id_t, Id_t> >&
                                    connections ) {
  for( auto& connection : connections ) {
    Id_t in_id = connection.first;
    Id_t out_id = connection.second;

    _json["connections"].push_back( in_id );
    _json["connections"].push_back( out_id );
  }
}

void JsonExporter::set_units_properties( const std::vector<LstmUnitProperties>&
                                         units_properties ) {
  for( auto& unit_properties : units_properties ) {
    json unit_json;

    unit_json["id"] = unit_properties.get_id();
    unit_json["type"] = unit_type_to_string( unit_properties.get_type() );
    unit_json["act_func"] =
      act_func_type_to_string( unit_properties.get_act_func_type() );

    if( unit_properties.get_self_conn() )
      unit_json["self_conn_gater"] = unit_properties.get_self_conn_gater();

    _json["units_properties"].push_back( unit_json );

    for( auto& gated_conn : unit_properties.get_gated_conns() ) {
      _json["gated_conns"].push_back( gated_conn.gater_id );
      _json["gated_conns"].push_back( gated_conn.in_id );
      _json["gated_conns"].push_back( gated_conn.out_id );    
    }
  }
}

void JsonExporter::set_weights( const WeightsMap_t& weights ) {
  if( _json.find( "connections" ) == _json.end() ) {
    string error = "connections must be exported before weights";
    throw JsonExporterException( error );
  }

  for( size_t i = 0; i < _json["connections"].size(); i += 2 ) {
    Id_t in_id = _json["connections"][i];
    Id_t out_id = _json["connections"][i + 1];

    _json["weights"].push_back( weights.at( out_id ).at ( in_id ) );
  }

  cout << "connections:" << endl;
  
  for( auto& x : _json["connections"] )
    cout << x << endl;

  cout << "weights:" << endl;

  for( auto& x : _json["weights"] )
    cout << x << endl;
}
