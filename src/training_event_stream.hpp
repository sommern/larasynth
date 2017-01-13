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
#include <utility>
#include <algorithm>

#include "training_sequence.hpp"
#include "training_sequence_parser.hpp"
#include "event.hpp"
#include "rand_gen.hpp"
#include "midi_min_max.hpp"

namespace larasynth {

/*
 * Trainers get data from a TrainingEventStream rather than from the
 * sequences directly. TrainingSequence objects are added to a
 * TrainingEventStream. The stream shuffles the order of the sequences,
 * scales the overall tempo, and adds some random local jitter to the tempo
 * between events. This produces a unique stream each time and produces more
 * generality during training.
 *  
 */
class TrainingEventStream {
public:
  explicit TrainingEventStream( size_t update_period,
                                double tempo_adjustment_factor,
                                double tempo_jitter_factor,
                                double mean_padding,
                                double padding_stddev,
                                ctrl_values_t default_ctrl_values );

  void add_sequence( const TrainingSequence& seq );
  void add_examples( const std::vector<std::string>& filenames );

  bool has_next() const { return _stream_i < _event_stream.size(); }
  Event get_next() { return _event_stream[_stream_i++]; }

  size_t get_next_type() { return _event_stream[_stream_i].type(); }
  size_t get_next_time() { return _event_stream[_stream_i].time(); }

  MidiMinMax get_min_max() { return _min_max; }

  void reset( size_t count );

private:
  double random_multiplier( double adjustment_factor );
  std::vector<Event> adjust_events( const std::vector<Event>&
                                    original_events );

  MidiMinMax _min_max;

  std::vector<TrainingSequence> _orig_seqs;

  std::vector<Event> _event_stream;
  std::size_t _stream_i;

  size_t _update_period;
  double _tempo_adjustment_factor;
  double _tempo_jitter_factor;
  size_t _mean_padding;
  size_t _padding_stddev;
  ctrl_values_t _default_ctrl_values;

  RandGen _rand_gen;
};

}
