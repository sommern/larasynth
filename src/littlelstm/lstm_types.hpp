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

#include <limits>
#include <utility>
#include <string>
#include <map>

typedef double Real_t;
typedef size_t Index_t;
typedef size_t Uint_t;
typedef size_t Id_t;

namespace littlelstm {

#define UNIT_TYPE_COUNT 11
enum lstm_unit_t {
  INPUT_LAYER,
  PREV_LAYER,
  INPUT_UNIT,
  INPUT_GATE,
  FORGET_GATE,
  OUTPUT_GATE,
  GATE_UNIT,
  CELL,
  BIAS_UNIT,
  OUTPUT_UNIT,
  ALL_UNITS,
  UNIT_ERROR
};

inline std::string unit_type_to_string( lstm_unit_t type ) {
  switch( type ) {
  case INPUT_UNIT:
    return "INPUT_UNIT";
  case INPUT_GATE:
    return "INPUT_GATE";
  case FORGET_GATE:
    return "FORGET_GATE";
  case OUTPUT_GATE:
    return "OUTPUT_GATE";
  case CELL:
    return "CELL";
  case BIAS_UNIT:
    return "BIAS_UNIT";
  case OUTPUT_UNIT:
    return "OUTPUT_UNIT";
  default:
    return "";
    break;
  } 
} 

inline lstm_unit_t string_to_unit_type( std::string str ) {
  if( str == "INPUT_UNIT" )
    return INPUT_UNIT;
  else if( str == "INPUT_GATE" )
    return INPUT_GATE;
  else if( str == "FORGET_GATE" )
    return FORGET_GATE;
  else if( str == "OUTPUT_GATE" )
    return OUTPUT_GATE;
  else if( str == "CELL" )
    return CELL;
  else if( str == "BIAS_UNIT" )
    return BIAS_UNIT;
  else if( str == "OUTPUT_UNIT" )
    return OUTPUT_UNIT;
  else
    return UNIT_ERROR;
}

enum lstm_act_func_t {
  IDENTITY,
  LOGISTIC,
  LOGISTIC_CENTERED,
  NO_ACTIVATION_FUNCTION
};

inline std::string act_func_type_to_string( lstm_act_func_t type ) {
  switch( type ) {
  case IDENTITY:
    return "IDENTITY";
  case LOGISTIC:
    return "LOGISTIC";
  case LOGISTIC_CENTERED:
    return "LOGISTIC_CENTERED";
  default:
    return "";
    break;
  }
}

inline lstm_act_func_t string_to_act_func_type( std::string str ) {
  if( str == "IDENTITY" )
    return IDENTITY;
  else if( str == "LOGISTIC" )
    return LOGISTIC;
  else if( str == "LOGISTIC_CENTERED" )
    return LOGISTIC_CENTERED;
  else
    return NO_ACTIVATION_FUNCTION;
}

#define WEIGHT_GEN_T_COUNT 5
enum lstm_weight_gen_t {
  NORMAL_WEIGHT_GEN,
  UNIFORM_WEIGHT_GEN,
  SEQUENCE_WEIGHT_GEN,
  INCREMENT_WEIGHT_GEN,
  NO_WEIGHTS
};

enum lstm_layer_t {
  INPUT,
  HIDDEN,
  OUTPUT,
  NONE
};
  
#define NO_UNIT std::numeric_limits<size_t>::max()

typedef Real_t (*act_func_ptr_t)(Real_t);
typedef std::pair<Id_t, Id_t> Conn_t;
typedef std::map< Id_t, std::map<Id_t, Real_t> > WeightsMap_t;

#define INPUT_LAYER_ID 0
#define BIAS_LAYER_ID 1
#define HIDDEN_LAYER_ID_OFFSET 2

}
