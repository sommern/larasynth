#include "rand_gen.h"
#include "gtest/gtest.h"

TEST( RandTest, RandomDoubleZeroOneTest ) {
  RandGen rg;

  for( size_t i = 0; i < 100; ++i ) {
    double curr = rg.uniform_real( 0.0, 1.0 );
    EXPECT_TRUE( curr >= 0.0 );
    EXPECT_TRUE( curr <= 1.0 );
  }
}

TEST( RandTest, RandomDoubleMinMaxTest ) {
  RandGen rg;

  for( size_t i = 0; i < 100; ++i ) {
    double curr = rg.uniform_real( -1.0, 100.0 );
    EXPECT_TRUE( curr >= -1.0 );
    EXPECT_TRUE( curr <= 100.0 );
  }
}

TEST( RandTest, RandomSizeTTest ) {
  RandGen rg;

  for( size_t i = 0; i < 100; ++i ) {
    size_t curr = rg.uniform_int<size_t>( 1, 100 );
    EXPECT_TRUE( curr >= 1 );
    EXPECT_TRUE( curr <= 100 );
  }
}

TEST( RandTest, Normal ) {
  RandGen rg;

  double sum = 0.0;
  size_t count = 100000;

  for( size_t i = 0; i < count; ++i )
    sum += rg.normal( 10.0, 5.0 );

  double mean = sum / count;

  EXPECT_TRUE( mean > 9.9 );
  EXPECT_TRUE( mean < 10.1 );
}
  

int main(int argc, char **argv) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
