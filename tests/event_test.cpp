#include "event.hpp"
#include "gtest/gtest.h"

using namespace larasynth;

class EventTest : public ::testing::Test {
protected:
  Event e;
};

TEST_F( EventTest, TestCtrlEvent ) {
  e.set_ctrl( 4, 20, 127, 10 );

  EXPECT_EQ( 4, e.channel() );
  EXPECT_EQ( 20, e.controller() );
  EXPECT_EQ( 127, e.value() );
  EXPECT_EQ( 10, e.time() );
  EXPECT_EQ( CTRL_CHANGE, e.type() );
}

TEST_F( EventTest, TestChangeCtrlValue ) {
  e.set_ctrl( 4, 20, 127, 10 );
  e.set_value( 0 );

  EXPECT_EQ( 0, e.value() );

  e.set_value( 60 );

  EXPECT_EQ( 60, e.value() );
  EXPECT_EQ( CTRL_CHANGE, e.type() );
}

TEST_F( EventTest, TestNoteOn ) {
  e.set_note_on( 15, 0, 127, 10000 );

  EXPECT_EQ( 15, e.channel() );
  EXPECT_EQ( 0, e.pitch() );
  EXPECT_EQ( 127, e.velocity() );
  EXPECT_EQ( 10000, e.time() );
  EXPECT_EQ( NOTE_ON, e.type() );
}

TEST_F( EventTest, TestChangeVelocity ) {
  e.set_note_on( 0, 127, 127, 30 );

  EXPECT_EQ( 127, e.velocity() );

  e.set_velocity( 0 );

  EXPECT_EQ( 0, e.velocity() );

  EXPECT_EQ( NOTE_ON, e.type() );
}

TEST_F( EventTest, TestChangeTime ) {
  e.set_note_on( 8, 88, 0, 40 );

  EXPECT_EQ( 40, e.time() );

  e.set_time( 100 );

  EXPECT_EQ( 100, e.time() );
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
