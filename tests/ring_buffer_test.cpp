#include "ring_buffer.h"
#include "gtest/gtest.h"

class RingBufferTest : public ::testing::Test {
protected:
  RingBufferTest()
    : rbi( 100 ),
      rbip( 50 )
  {}

  RingBuffer<int> rbi;
  RingBuffer<int*> rbip;
};

TEST_F( RingBufferTest, TestEmptyRingBuffer ) {
  EXPECT_TRUE( rbi.empty() );
}

TEST_F( RingBufferTest, TestPush ) {
  rbi.push( 1 );
  EXPECT_FALSE( rbi.empty() );
  EXPECT_EQ( 1, rbi.front() );
  int i = rbi.front();
  EXPECT_EQ( 1, i );
  int i2 = rbi.front_pop();
  EXPECT_EQ( 1, i2 );
  EXPECT_TRUE( rbi.empty() );
}

TEST_F( RingBufferTest, TestPushPointer ) {
  int* ip = new int;
  *ip = 10;
  rbip.push( ip );
  EXPECT_FALSE( rbip.empty() );
  int* ip2 = rbip.front();
  EXPECT_EQ( ip, ip2 );
  EXPECT_EQ( *ip, *ip2 );
  delete ip;
}

TEST_F( RingBufferTest, TestPushManyWithDelete ) {
  size_t push_count = 100;

  for( size_t i = 0; i < push_count; ++i )
    rbip.push_or_delete_if_full( new int( i ) );

  EXPECT_FALSE( rbip.empty() );

  int* ip;
  size_t pop_count = 0;

  while( !rbip.empty() ) {
    ip = rbip.front_pop();
    delete ip;
    ++pop_count;
  }

  ASSERT_LT( pop_count, push_count );
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
