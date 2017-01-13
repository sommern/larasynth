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

#include "lstm_architecture.hpp"

using namespace std;
using namespace larasynth;

LstmArchitecture::LstmArchitecture( LstmConfig& config )
  : _input_count( config.get_input_count() ),
    _output_count( config.get_output_count() ),
    _next_id( 0 )
{
  size_t hidden_layer_count = config.get_hidden_layer_count();
  vector<size_t> block_counts = config.get_block_counts();
  vector<size_t> cells_per_block = config.get_cells_per_block();
  
  // vector< vector< vector<LstmWeightConfig> > > weight_configs
  //   = config.get_weight_configs();

  _unit_count = _input_count + _output_count + 1;
  for( size_t h = 0; h < hidden_layer_count; ++h ) {
    for( size_t b = 0; b < block_counts[h]; ++b ) {
      _unit_count += 3;
      for( size_t c = 0; c < cells_per_block[h]; ++c ) {
        _unit_count += 1;
      }
    }
  }

  // add all units and add IDs to vectors
  for( size_t i = 0; i < _input_count; ++i )
    _input_ids.push_back( add_unit( INPUT_UNIT ) );
  
  Id_t bias_id = add_unit( BIAS_UNIT );

  for( size_t h = 0; h < hidden_layer_count; ++h ) {
    _input_gate_ids.push_back( vector<Id_t>() );
    _forget_gate_ids.push_back( vector<Id_t>() );
    _output_gate_ids.push_back( vector<Id_t>() );
    _all_gate_ids.push_back( vector<Id_t>() );
    _block_cell_ids.push_back( vector< vector<Id_t> >() );
    _all_cell_ids.push_back( vector<Id_t>() );

    for( size_t b = 0; b < block_counts[h]; ++b ) {
      _input_gate_ids[h].push_back( add_unit( INPUT_GATE, LOGISTIC ) );
      _forget_gate_ids[h].push_back( add_unit( FORGET_GATE, LOGISTIC ) );
      _output_gate_ids[h].push_back( add_unit( OUTPUT_GATE, LOGISTIC ) );

      _all_gate_ids[h].push_back( _input_gate_ids[h].back() );
      _all_gate_ids[h].push_back( _forget_gate_ids[h].back() );
      _all_gate_ids[h].push_back( _output_gate_ids[h].back() );

      _block_cell_ids[h].push_back( vector<Id_t>() );

      for( size_t c = 0; c < cells_per_block[h]; ++c ) {
        Id_t cell_id = add_unit( CELL, LOGISTIC );
        _block_cell_ids[h][b].push_back( cell_id );
        _all_cell_ids[h].push_back( cell_id );
        _gated_cell_inputs[cell_id] = vector<Id_t>();
        _gated_cell_outputs[cell_id] = vector<Id_t>();
      }
    }
  }

  for( size_t o = 0; o < _output_count; ++o )
    _output_ids.push_back( add_unit( OUTPUT_UNIT, LOGISTIC ) );

  // add connections

  // connect input to hidden layers
  for( Index_t h = 0; h < hidden_layer_count; ++h ) {
    Id_t h_id = h + HIDDEN_LAYER_ID_OFFSET;

    for( Id_t in_id = 0; in_id < _input_count; ++in_id ) {
      if( config.connection_needed( INPUT_LAYER_ID, h_id, INPUT_UNIT,
                                    INPUT_GATE ) ) {
        for( auto& out_id : _input_gate_ids[h] )
          add_connection( in_id, out_id );
      }
      if( config.connection_needed( INPUT_LAYER_ID, h_id, INPUT_UNIT,
                                    FORGET_GATE ) ) {
        for( auto& out_id : _forget_gate_ids[h] )
          add_connection( in_id, out_id );
      }
      if( config.connection_needed( INPUT_LAYER_ID, h_id, INPUT_UNIT,
                                    OUTPUT_GATE ) ) {
        for( auto& out_id : _output_gate_ids[h] )
          add_connection( in_id, out_id );
      }
      if( config.connection_needed( INPUT_LAYER_ID, h_id, INPUT_UNIT,
                                    CELL ) ) {
        for( auto& out_id : _all_cell_ids[h] ) {
          add_connection( in_id, out_id );
          _gated_cell_inputs[out_id].push_back( in_id );
        }
      }
    }
  }

  // connect bias to hidden layers
  for( Index_t h = 0; h < hidden_layer_count; ++h ) {
    Id_t h_id = h + HIDDEN_LAYER_ID_OFFSET;
    
    if( config.connection_needed( BIAS_LAYER_ID, h_id, BIAS_UNIT,
                                  INPUT_GATE ) ) {
      for( auto& out_id : _input_gate_ids[h] )
        add_connection( bias_id, out_id );
    }
    if( config.connection_needed( BIAS_LAYER_ID, h_id, BIAS_UNIT,
                                  FORGET_GATE ) ) {
      for( auto& out_id : _forget_gate_ids[h] )
        add_connection( bias_id, out_id );
    }
    if( config.connection_needed( BIAS_LAYER_ID, h_id, BIAS_UNIT,
                                  OUTPUT_GATE ) ) {
      for( auto& out_id : _output_gate_ids[h] )
        add_connection( bias_id, out_id );
    }
    if( config.connection_needed( BIAS_LAYER_ID, h_id, BIAS_UNIT, CELL ) ) {
      for( auto& out_id : _all_cell_ids[h] ) {
        add_connection( bias_id, out_id );
        _gated_cell_inputs[out_id].push_back( bias_id );
      }
    }
  }


  // connect each hidden layer's input, forget, and output gates
  for( Index_t h = 0; h < hidden_layer_count; ++h ) {
    Id_t h_id = h + HIDDEN_LAYER_ID_OFFSET;
    
    for( lstm_unit_t in_type : { INPUT_GATE, FORGET_GATE, OUTPUT_GATE } ) {
      for( lstm_unit_t out_type : { INPUT_GATE, FORGET_GATE, OUTPUT_GATE } ) {
        if( !config.connection_needed( h_id, h_id, in_type, out_type ) )
          continue;

        const vector<Id_t> in_ids = get_layer_ids_by_type( h, in_type );
        const vector<Id_t> out_ids = get_layer_ids_by_type( h, out_type );

        for( auto& in_id : in_ids ) {
          for( auto& out_id : out_ids )
            add_connection( in_id, out_id );
        }
      }
    }
  }

  // connect each hidden layer's gates to cells
  for( Index_t h = 0; h < hidden_layer_count; ++h ) {
    Id_t h_id = h + HIDDEN_LAYER_ID_OFFSET;

    lstm_unit_t out_type = CELL;
    for( lstm_unit_t in_type : { INPUT_GATE, FORGET_GATE, OUTPUT_GATE } ) {
      if( !config.connection_needed( h_id, h_id, in_type, out_type ) )
        continue;

      const vector<Id_t> in_ids = get_layer_ids_by_type( h, in_type );
      const vector<Id_t> out_ids = get_layer_ids_by_type( h, out_type );

      for( auto& in_id : in_ids ) {
        for( auto& out_id : out_ids ) {
          add_connection( in_id, out_id );
          _gated_cell_inputs[out_id].push_back( in_id );
        }
      }
    }
  }

  // add peephole connections to gates and cells in each hidden layer
  for( Index_t h = 0; h < hidden_layer_count; ++h ) {
    Id_t h_id = h + HIDDEN_LAYER_ID_OFFSET;
    
    for( lstm_unit_t out_type :
      { INPUT_GATE, FORGET_GATE, OUTPUT_GATE, CELL } ) {
      if( !config.connection_needed( h_id, h_id, CELL, out_type ) )
        continue;

      const vector<Id_t> out_ids = get_layer_ids_by_type( h, out_type );

      for( auto& in_id : _all_cell_ids[h] ) {
        for( auto& out_id : out_ids ) {
          if( in_id != out_id )
            add_connection( in_id, out_id );
        }
      }
    }
  }

  // connect hidden layers to previous layers
  for( Index_t h_out = 1; h_out < hidden_layer_count; ++h_out ) {
    Index_t h_in = h_out - 1;

    Id_t h_out_id = h_out + HIDDEN_LAYER_ID_OFFSET;
    Id_t h_in_id = h_in + HIDDEN_LAYER_ID_OFFSET;

    // cout << "in id: " << h_in_id << endl;
    // cout << "out id: " << h_out_id << endl;

    for( lstm_unit_t out_type : { INPUT_GATE, FORGET_GATE, OUTPUT_GATE,
          CELL } ) {
      if( !config.connection_needed( h_in_id, h_out_id, CELL, out_type ) )
        continue;

      const vector<Id_t> out_ids = get_layer_ids_by_type( h_out, out_type );

      for( auto& in_id : _all_cell_ids[h_in] ) {
        for( auto& out_id : out_ids ) {
          add_connection( in_id, out_id );
          _gated_cell_outputs[in_id].push_back( out_id );
          if( out_type == CELL )
            _gated_cell_inputs[out_id].push_back( in_id );
        }
      }
    }
  }

  // connect hidden layers to output layer
  Id_t output_layer_id = BIAS_LAYER_ID + hidden_layer_count + 1;

  for( Index_t h = 0; h < hidden_layer_count; ++h ) {
    Id_t h_id = h + HIDDEN_LAYER_ID_OFFSET;
    
    if( !config.connection_needed( h_id, output_layer_id, CELL,
                                   OUTPUT_UNIT ) )
      continue;
    
    for( auto& in_id : _all_cell_ids[h] ) {
      for( auto& out_id : _output_ids ) {
        add_connection( in_id, out_id );
        _gated_cell_outputs[in_id].push_back( out_id );
      }
    }
  }


  // add gates
  for( Index_t h = 0; h < hidden_layer_count; ++h ) {
    for( Index_t b = 0; b < block_counts[h]; ++b ) {
      Id_t input_gate_id = _input_gate_ids[h][b];
      Id_t forget_gate_id = _forget_gate_ids[h][b];
      Id_t output_gate_id = _output_gate_ids[h][b];

      for( Index_t c = 0; c < cells_per_block[h]; ++c ) {
        Id_t cell_id = _block_cell_ids[h][b][c];

        add_self_gate( cell_id, forget_gate_id );

        for( auto& in_id : _gated_cell_inputs[cell_id] ) {
          // cout << "gate " << input_gate_id << " gating " << in_id << " -> "
          //      << cell_id << endl;
          add_gate( in_id, cell_id, input_gate_id );
        }
        for( auto& out_id : _gated_cell_outputs[cell_id] )
          add_gate( cell_id, out_id, output_gate_id );
      }
    }
  }

  // connect bias to output layer
  if( config.connection_needed( BIAS_LAYER_ID, output_layer_id, BIAS_UNIT,
                                OUTPUT_UNIT ) ) {
    for( auto& out_id : _output_ids )
      add_connection( bias_id, out_id );
  }
}

void LstmArchitecture::add_connection( Id_t in_id, Id_t out_id ) {
  _connections.push_back( Conn_t( in_id, out_id ) );
}

Id_t LstmArchitecture::add_unit( lstm_unit_t type,
                                  lstm_act_func_t act_func ) {
  _units_properties.push_back( LstmUnitProperties( _next_id, type ) );
  _units_properties[_next_id].set_act_func( act_func );
    
  return _next_id++;
}

void LstmArchitecture::add_gate( Id_t in_id, Id_t out_id, Id_t gate_id ) {
  _units_properties[gate_id].add_gated_conn( in_id, out_id );
}

void LstmArchitecture::add_self_gate( Id_t cell_id, Id_t gate_id ) {
  _units_properties[cell_id].set_self_conn_gater( gate_id );
}

vector<Id_t> LstmArchitecture::get_layer_ids_by_type( Index_t h,
                                                       lstm_unit_t type ) {
  switch( type ) {
  case INPUT_GATE:
    return _input_gate_ids[h];
    break;
  case FORGET_GATE:
    return _forget_gate_ids[h];
    break;
  case OUTPUT_GATE:
    return _output_gate_ids[h];
    break;
  case CELL:
    return _all_cell_ids[h];
    break;
  default:
    assert( false );
    break;
  }

  return vector<Id_t>();
}
