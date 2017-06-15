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

#include <vector>
#include <cmath>

#include "lstm_types.hpp"
#include "rand_gen.hpp"

namespace littlelstm {

struct LstmWeightConfig {
  LstmWeightConfig( lstm_weight_gen_t _type, Real_t _first, Real_t _second ) 
    : type( _type ), first( _first ), second( _second ) {}
  LstmWeightConfig( const std::vector<Real_t>& seq )
    : type( SEQUENCE_WEIGHT_GEN ), sequence( seq ) {}
  LstmWeightConfig() : type( NO_WEIGHTS ) {}

  lstm_weight_gen_t type;
  Real_t first;
  Real_t second;
  std::vector<Real_t> sequence;
};

class LstmWeightGenerator {
public:
  virtual ~LstmWeightGenerator() {};
  virtual double next_weight() =0;
  virtual void reset() =0;
};

class LstmWeightGenRange : public LstmWeightGenerator {
public:
  LstmWeightGenRange( double min, double max, RandGen* rand_gen )
    : _min( min ), _max( max ), _rand_gen( rand_gen ) {}
  LstmWeightGenRange( double min, double max )
    : _min( min ), _max( max ), _rand_gen( new RandGen ) {}
  ~LstmWeightGenRange() {}

  double next_weight() { return _rand_gen->uniform_real( _min, _max ); }
  void reset() {}

private:
  double _min;
  double _max;
  RandGen* _rand_gen;
};

class LstmWeightGenInc : public LstmWeightGenerator {
public:
  LstmWeightGenInc( double first, double increment )
    : _first( first ), _increment( increment ), _current( first - increment ) {}
  ~LstmWeightGenInc() {}

  double next_weight() { return ( _current += _increment ); }
  void reset() { _current = _first - _increment; }

private:
  double _first;
  double _increment;
  double _current;
};

class LstmWeightGenSeq : public LstmWeightGenerator {
public:
  LstmWeightGenSeq( std::vector<double>& sequence )
    : _sequence( sequence ), _index( 0 ) {}
  ~LstmWeightGenSeq() {}

  double next_weight() {
    if( _index == _sequence.size() )
      _index = 0;
    return _sequence[ _index++ ];
  }
  void reset() { _index = 0; }

private:
  std::vector<double> _sequence;
  size_t _index;
};

class LstmWeightGenRead : public LstmWeightGenerator {
public:
  LstmWeightGenRead( std::vector<double>& weights )
    : _weights( weights ), _index( 0 ) {}
  ~LstmWeightGenRead() {}

  double next_weight() {
    if( _index == _weights.size() )
      return INFINITY;
    return _weights[ _index++ ];
  }
  void reset() { _index = 0; }

private:
  std::vector<double> _weights;
  size_t _index;
};

}
