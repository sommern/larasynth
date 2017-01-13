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

#include "training_event_stream.hpp"

using namespace std;
using namespace larasynth;

TrainingEventStream::TrainingEventStream( size_t update_period,
                                          double tempo_adjustment_factor,
                                          double tempo_jitter_factor,
                                          double mean_padding,
                                          double padding_stddev,
                                          ctrl_values_t default_ctrl_values )
  : _stream_i( 0 )
  , _update_period( update_period )
  , _tempo_adjustment_factor( tempo_adjustment_factor )
  , _tempo_jitter_factor( tempo_jitter_factor )
  , _mean_padding( mean_padding * MICROSECONDS_PER_SECOND )
  , _padding_stddev( padding_stddev * MICROSECONDS_PER_SECOND )
  , _default_ctrl_values( default_ctrl_values )
{}

void TrainingEventStream::add_sequence( const TrainingSequence& seq ) {
  _orig_seqs.push_back( seq );
  _min_max.consider_sequence( seq );
}

void TrainingEventStream::add_examples( const vector<string>& filenames ) {
  for( const string& filename : filenames ) {
    TrainingSequenceParser tsp( filename );
    TrainingSequence seq( tsp.get_sequence() );
    add_sequence( seq );
  }
}

double TrainingEventStream::random_multiplier( double adjustment_factor ) {
  if( adjustment_factor == 0.0 )
    return 1.0;

  // multiplier >= 1.0
  if( _rand_gen.uniform_int( 0, 1 ) == 0 )
    return 1.0 + _rand_gen.uniform_real( 0.0, adjustment_factor );
  // multipler <= 1.0
  else
    return 1.0 - ( _rand_gen.uniform_real( 0.0, adjustment_factor ) / 2 );
}

vector<Event> TrainingEventStream::adjust_events( const vector<Event>&
                                                  original_events ) {
  vector<Event> adjusted_events;
  adjusted_events.reserve( original_events.size() );

  double tempo_multiplier = random_multiplier( _tempo_adjustment_factor );

  size_t first_event_time = original_events[0].time();
  size_t last_event_time = first_event_time;

  size_t last_event_time_adjusted = 0;

  for( Event event : original_events ) {
    double jitter = random_multiplier( _tempo_jitter_factor );

    size_t duration_from_last = event.time() - last_event_time;

    size_t adjusted_duration_from_last =
      duration_from_last * tempo_multiplier * jitter;

    size_t adjusted_time =
      last_event_time_adjusted + adjusted_duration_from_last;

    last_event_time = event.time();
    last_event_time_adjusted = adjusted_time;
      
    event.set_time( adjusted_time );
    adjusted_events.push_back( event );
  }

  return adjusted_events;
}

void TrainingEventStream::reset( size_t count ) {
  vector<TrainingSequence> seqs;

  for( auto& seq : _orig_seqs ) {
    for( size_t i = 0; i < count; ++i )
      seqs.emplace_back( seq.get_events() );
  }

  shuffle( seqs.begin(), seqs.end(), *_rand_gen.get_engine_ptr() );

  _event_stream.resize( 0 );
  _stream_i = 0;

  size_t last_event_time = 0;
  size_t next_event_time = 0;

  for( auto& seq : seqs ) {
    const vector<Event>& original_events = seq.get_events_ref();

    vector<Event> adjusted_events = adjust_events( original_events );

    if( _mean_padding > 0 || _padding_stddev > 0 ) {
      for( auto& kv : _default_ctrl_values ) {
        Event padding_event;
        padding_event.set_ctrl( 0, kv.first, kv.second, next_event_time );
        _event_stream.push_back( padding_event );
        
        ++last_event_time;
        next_event_time = last_event_time + 1;
      }

      long long int padding_time = _rand_gen.normal( (double)_mean_padding,
                                                     (double)_padding_stddev );

      // could be negative, but the next event must be after the previous one
      if( padding_time <= 0 )
        padding_time = 1;

      next_event_time = last_event_time + padding_time;
    }
    else if( last_event_time != 0 )
      next_event_time = last_event_time + _update_period;

    long long int time_offset = next_event_time;

    for( Event event : adjusted_events ) {
      event.set_time( event.time() + time_offset );
      last_event_time = event.time();
      _event_stream.push_back( event );
    }

    next_event_time = last_event_time + _update_period;
  }
}
