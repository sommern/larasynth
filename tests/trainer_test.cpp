#include <string>

#include "trainer.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace larasynth;
using namespace littlelstm;

TEST( LstmTrainerTest, SimpleSequence ) {
  string directory = "test_files/trainer_test";
  volatile sig_atomic_t shutdown_flag = false;

  Trainer trainer( directory, &shutdown_flag );
}

void construct_trainer( const string& directory ) {
  volatile sig_atomic_t shutdown_flag = false;

  Trainer trainer( directory, &shutdown_flag );
}


TEST( LstmTrainerTest, NoExamples ) {
  string directory = "test_files/no_examples";

  ASSERT_THROW( construct_trainer( directory ), runtime_error );
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
