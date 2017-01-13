#include "midi_state.hpp"
#include "midi_config.hpp"
#include "config_parser.hpp"
#include "config_parameters.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace larasynth;

TEST( MidiStateTest, ThreeControllers ) {
  ctrl_values_t ctrl_defaults = { { 1, 64 }, { 2, 127 }, { 3, 0 } };

  MidiState midi_state( ctrl_defaults );

  ctrl_values_t ctrl_values = midi_state.get_ctrl_values();
  vector<event_data_t> note_velocities = midi_state.get_note_velocities();

  EXPECT_FALSE( midi_state.any_notes_on() );

  for( auto vel : note_velocities )
    EXPECT_EQ( 0, vel );

  EXPECT_EQ( 64, ctrl_values[1] );
  EXPECT_EQ( 127, ctrl_values[2] );
  EXPECT_EQ( 0, ctrl_values[3] );

  Event event;

  event.set_note_on( 0, 60, 100, 0 );
  midi_state.new_event( event );

  note_velocities = midi_state.get_note_velocities();

  EXPECT_TRUE( midi_state.any_notes_on() );

  size_t on_count = 0;
  for( auto vel : note_velocities ) {
    if( vel != 0 )
      ++on_count;
  }

  EXPECT_EQ( 1, on_count );

  EXPECT_EQ( 100, note_velocities[60] );
    
  EXPECT_EQ( 64, ctrl_values[1] );
  EXPECT_EQ( 127, ctrl_values[2] );
  EXPECT_EQ( 0, ctrl_values[3] );

  event.set_ctrl( 0, 2, 70, 0 );
  midi_state.new_event( event );
  
  note_velocities = midi_state.get_note_velocities();
  ctrl_values = midi_state.get_ctrl_values();

  EXPECT_TRUE( midi_state.any_notes_on() );

  on_count = 0;
  for( auto vel : note_velocities ) {
    if( vel != 0 )
      ++on_count;
  }

  EXPECT_EQ( 1, on_count );
    
  EXPECT_EQ( 64, ctrl_values[1] );
  EXPECT_EQ( 70, ctrl_values[2] );
  EXPECT_EQ( 0, ctrl_values[3] );
}

TEST( MidiStateTest, OneController ) {
  ctrl_values_t ctrl_defaults = { { 2, 60 } };

  MidiState midi_state( ctrl_defaults );

  ctrl_values_t ctrl_values = midi_state.get_ctrl_values();
  vector<event_data_t> note_velocities = midi_state.get_note_velocities();

  EXPECT_FALSE( midi_state.any_notes_on() );

  for( auto vel : note_velocities )
    EXPECT_EQ( 0, vel );

  EXPECT_EQ( 60, ctrl_values[2] );

  Event event;

  event.set_note_on( 0, 60, 100, 0 );
  midi_state.new_event( event );

  note_velocities = midi_state.get_note_velocities();

  EXPECT_TRUE( midi_state.any_notes_on() );

  size_t on_count = 0;
  for( auto vel : note_velocities ) {
    if( vel != 0 )
      ++on_count;
  }

  EXPECT_EQ( 1, on_count );

  EXPECT_EQ( 100, note_velocities[60] );
    
  EXPECT_EQ( 60, ctrl_values[2] );

  event.set_ctrl( 0, 2, 70, 0 );
  midi_state.new_event( event );
  
  note_velocities = midi_state.get_note_velocities();
  ctrl_values = midi_state.get_ctrl_values();

  EXPECT_TRUE( midi_state.any_notes_on() );

  on_count = 0;
  for( auto vel : note_velocities ) {
    if( vel != 0 )
      ++on_count;
  }

  EXPECT_EQ( 1, on_count );
    
  EXPECT_EQ( 70, ctrl_values[2] );
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
