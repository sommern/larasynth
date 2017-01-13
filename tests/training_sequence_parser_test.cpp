#include <string>
#include <vector>
#include <utility>

#include "training_sequence_parser.hpp"
#include "event.hpp"
#include "gtest/gtest.h"

using namespace std;
using namespace larasynth;

TEST( TrainingSequenceParserTest, AllThings ) {
  string filename = "test_files/training_sequence_parser_test/test.seq";
  TrainingSequenceParser tsp( filename );

  TrainingSequence seq = tsp.get_sequence();

  vector<Event> events = seq.get_events();

  EXPECT_EQ( 87, events.size() );

  EXPECT_EQ( 1, seq.get_count_min() );
  EXPECT_EQ( 5, seq.get_count_max() );
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
