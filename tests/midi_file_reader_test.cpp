#include "midi_file_reader.hpp"
#include "gtest/gtest.h"

using namespace larasynth;

TEST( MidifileTest, ReadFile ) {
  vector<event_data_t> ctrls = { 1, 91 };

  MidiFileReader reader( "test_files/midi_file_reader_test/midi_file_reader_test.mid", ctrls );

  EXPECT_EQ( reader.get_track_count(), 4 );
  EXPECT_EQ( reader.get_note_on_count(), 3 );
  EXPECT_EQ( reader.get_ctrl_change_count(), 4 );

  vector<Event> events = reader.get_events();

  size_t note_on_count = 0;
  size_t note_off_count = 0;
  size_t ctrl_count = 0;

  for( Event& event : events ) {
    if( event.type() == NOTE_ON )
      ++note_on_count;
    else if( event.type() == NOTE_OFF )
      ++note_off_count;
    else if( event.type() == CTRL_CHANGE )
      ++ctrl_count;
  }

  EXPECT_EQ( note_on_count, 3 );
  EXPECT_EQ( note_off_count, 3 );
  EXPECT_EQ( ctrl_count, 4 );
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
