#include "training_config.hpp"
#include "config_parser.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace larasynth;

class TrainingConfigTest : public ::testing::Test {
protected:
  string prefix = "test_files/training_config_test/";
};

TEST_F( TrainingConfigTest, Defaults ) {
  ConfigParser cp( prefix + "defaults/larasynth.conf" );

  ConfigParameters params = cp.get_section_params( "training" );
  
  TrainingConfig config( params );

  EXPECT_EQ( 2, config.get_example_repetitions() );
  EXPECT_EQ( 5, config.get_validation_example_repetitions() );
  EXPECT_EQ( 50, config.get_epoch_count_before_validating() );
  EXPECT_EQ( 0.0, config.get_tempo_adjustment_factor() );
  EXPECT_EQ( 0.0, config.get_tempo_jitter_factor() );  
  EXPECT_EQ( 0.0, config.get_mean_padding() );
  EXPECT_EQ( 0.0, config.get_padding_stddev() );
  EXPECT_EQ( 1.0, config.get_zero_network_before_each_epoch() );
  EXPECT_TRUE( config.get_feed_back_output() );
  EXPECT_EQ( 0.0, config.get_backpropagate_if_correct() );
  EXPECT_EQ( 1.0, config.get_reset_probability() );
  EXPECT_EQ( 1.0, config.get_zero_network_before_validation() );
  EXPECT_EQ( 1, config.get_consecutive_failures_for_reset() );
  EXPECT_EQ( 0.0, config.get_squared_error_failure_tolerance() );
  EXPECT_EQ( 1.0, config.get_zero_network_on_reset() );
  EXPECT_EQ( 10000, config.get_max_epoch_count() );
  EXPECT_EQ( 0.0, config.get_mse_threshold() );
}

TEST_F( TrainingConfigTest, Values ) {
  ConfigParser cp( prefix + "values/larasynth.conf" );

  ConfigParameters params = cp.get_section_params( "training" );
  
  TrainingConfig config( params );

  EXPECT_EQ( 100, config.get_epoch_count_before_validating() );
  EXPECT_EQ( 0.1, config.get_zero_network_before_each_epoch() );
  EXPECT_FALSE( config.get_feed_back_output() );
  EXPECT_EQ( 0.01, config.get_backpropagate_if_correct() );
  EXPECT_EQ( 0.99, config.get_reset_probability() );
  EXPECT_EQ( 0.9, config.get_zero_network_before_validation() );
  EXPECT_EQ( 10, config.get_consecutive_failures_for_reset() );
  EXPECT_EQ( 4, config.get_squared_error_failure_tolerance() );
  EXPECT_EQ( 0.9, config.get_zero_network_on_reset() );
  EXPECT_EQ( 5000, config.get_max_epoch_count() );
  EXPECT_EQ( 0.1, config.get_mse_threshold() );
  EXPECT_EQ( 1.0, config.get_mean_padding() );
  EXPECT_EQ( 0.1, config.get_padding_stddev() );
}

TEST_F( TrainingConfigTest, UniformRandPosInt ) {
  for( size_t i = 0; i < 100; ++i ) {
    ConfigParser cp( prefix + "uniform_rand_positive_int/larasynth.conf" );

    ConfigParameters params = cp.get_section_params( "training" );
  
    TrainingConfig config( params );

    EXPECT_LE( 5, config.get_consecutive_failures_for_reset() );
    EXPECT_GE( 15, config.get_consecutive_failures_for_reset() );
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
