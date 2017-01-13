#include <string>
#include <iostream>

#include "training_event_stream.hpp"
#include "event.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace larasynth;

/**
 * This simply tests that a TrainingEventStream object can be constructed,
 * reset, and that has_next() and get_next() can be called repeatedly and
 * eventually the sequence is exhausted. There is no attempt to verify correct
 * values.
 */
TEST( TrainingEventStreamTest, OneSeqNoJitter ) {
  vector<string> filenames = { "test_files/training_event_stream_test/test.seq", "test_files/training_event_stream_test/test2.seq" };

  TrainingEventStream stream( 100, 0.5, 0.2, 1.0, 0.1, { { 3, 0 } } );

  stream.add_examples( filenames );
  stream.reset( 10 );

  while( stream.has_next() )
    stream.get_next();

  stream.reset( 20 );

  while( stream.has_next() )
    stream.get_next();

  stream.reset( 1 );
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

    
