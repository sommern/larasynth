AM_CPPFLAGS = -isystem $(top_srcdir)/lib/gtest/include -I$(top_srcdir)/src $(GTEST_CPPFLAGS)
AM_CXXFLAGS = $(GTEST_CXXFLAGS)
AM_LDFLAGS = $(GTEST_LDFLAGS) $(GTEST_LIBS) \
	     $(top_srcdir)/lib/gtest/lib/libgtest_main.la \
	     $(top_srcdir)/lib/gtest/lib/libgtest.la


# TESTS -- Programs run automatically by "make check"
# check_PROGRAMS -- Programs built by "make check" but not necessarily run
TESTS = 
check_PROGRAMS = 

TESTS += event_test
check_PROGRAMS += event_test
event_test_SOURCES = event_test.cpp
event_test_LDADD = $(top_srcdir)/src/event.o

TESTS += tokens_test
check_PROGRAMS += tokens_test
tokens_test_SOURCES = tokens_test.cpp
tokens_test_LDADD = $(top_srcdir)/src/tokens.o

TESTS += config_directory_test
check_PROGRAMS += config_directory_test
config_directory_test_SOURCES = config_directory_test.cpp
config_directory_test_LDADD = $(top_srcdir)/src/config_directory.o

TESTS += config_parser_test
check_PROGRAMS += config_parser_test
config_parser_test_SOURCES = config_parser_test.cpp
config_parser_test_LDADD = $(top_srcdir)/src/config_parser.o
config_parser_test_LDADD += $(top_srcdir)/src/tokens.o
config_parser_test_LDADD += $(top_srcdir)/src/config_parameter.o
config_parser_test_LDADD += $(top_srcdir)/src/config_parameters.o
config_parser_test_LDADD += $(top_srcdir)/src/lexer.o

TESTS += lstm_config_test
check_PROGRAMS += lstm_config_test
lstm_config_test_SOURCES = lstm_config_test.cpp
lstm_config_test_LDADD = $(top_srcdir)/src/lstm_config.o
lstm_config_test_LDADD += $(top_srcdir)/src/config_parser.o
lstm_config_test_LDADD += $(top_srcdir)/src/config_parameter.o
lstm_config_test_LDADD += $(top_srcdir)/src/config_parameters.o
lstm_config_test_LDADD += $(top_srcdir)/src/lstm_layer_config.o
lstm_config_test_LDADD += $(top_srcdir)/src/lexer.o
lstm_config_test_LDADD += $(top_srcdir)/src/tokens.o

TESTS += training_config_test
check_PROGRAMS += training_config_test
training_config_test_SOURCES = training_config_test.cpp
training_config_test_LDADD = $(top_srcdir)/src/training_config.o
training_config_test_LDADD += $(top_srcdir)/src/config_parser.o
training_config_test_LDADD += $(top_srcdir)/src/config_parameter.o
training_config_test_LDADD += $(top_srcdir)/src/config_parameters.o
training_config_test_LDADD += $(top_srcdir)/src/lexer.o
training_config_test_LDADD += $(top_srcdir)/src/tokens.o

TESTS += training_sequence_parser_test
check_PROGRAMS += training_sequence_parser_test
training_sequence_parser_test_SOURCES = training_sequence_parser_test.cpp
training_sequence_parser_test_LDADD = $(top_srcdir)/src/training_sequence_parser.o
training_sequence_parser_test_LDADD += $(top_srcdir)/src/event.o
training_sequence_parser_test_LDADD += $(top_srcdir)/src/training_sequence.o
training_sequence_parser_test_LDADD += $(top_srcdir)/src/lexer.o
training_sequence_parser_test_LDADD += $(top_srcdir)/src/tokens.o

TESTS += midi_config_test
check_PROGRAMS += midi_config_test
midi_config_test_SOURCES = midi_config_test.cpp
midi_config_test_LDADD = $(top_srcdir)/src/midi_config.o
midi_config_test_LDADD += $(top_srcdir)/src/config_directory.o
midi_config_test_LDADD += $(top_srcdir)/src/config_parser.o
midi_config_test_LDADD += $(top_srcdir)/src/config_parameters.o
midi_config_test_LDADD += $(top_srcdir)/src/config_parameter.o
midi_config_test_LDADD += $(top_srcdir)/src/lexer.o
midi_config_test_LDADD += $(top_srcdir)/src/tokens.o

TESTS += representation_config_test
check_PROGRAMS += representation_config_test
representation_config_test_SOURCES = representation_config_test.cpp
representation_config_test_LDADD = $(top_srcdir)/src/representation_config.o
representation_config_test_LDADD += $(top_srcdir)/src/config_parser.o
representation_config_test_LDADD += $(top_srcdir)/src/config_parameter.o
representation_config_test_LDADD += $(top_srcdir)/src/config_parameters.o
representation_config_test_LDADD += $(top_srcdir)/src/lexer.o
representation_config_test_LDADD += $(top_srcdir)/src/tokens.o

TESTS += training_event_stream_test
check_PROGRAMS += training_event_stream_test
training_event_stream_test_SOURCES = training_event_stream_test.cpp
training_event_stream_test_LDADD = $(top_srcdir)/src/representation_config.o
training_event_stream_test_LDADD += $(top_srcdir)/src/config_parser.o
training_event_stream_test_LDADD += $(top_srcdir)/src/config_parameter.o
training_event_stream_test_LDADD += $(top_srcdir)/src/config_parameters.o
training_event_stream_test_LDADD += $(top_srcdir)/src/lexer.o
training_event_stream_test_LDADD += $(top_srcdir)/src/tokens.o
training_event_stream_test_LDADD += $(top_srcdir)/src/event.o
training_event_stream_test_LDADD += $(top_srcdir)/src/midi_config.o
training_event_stream_test_LDADD += $(top_srcdir)/src/training_event_stream.o
training_event_stream_test_LDADD += $(top_srcdir)/src/midi_min_max.o
training_event_stream_test_LDADD += $(top_srcdir)/src/midi_translator.o
training_event_stream_test_LDADD += $(top_srcdir)/src/training_sequence.o
training_event_stream_test_LDADD += $(top_srcdir)/src/training_sequence_parser.o

TESTS += midi_state_test
check_PROGRAMS += midi_state_test
midi_state_test_SOURCES = midi_state_test.cpp
midi_state_test_LDADD = $(top_srcdir)/src/midi_state.o
midi_state_test_LDADD += $(top_srcdir)/src/midi_config.o
midi_state_test_LDADD += $(top_srcdir)/src/config_directory.o
midi_state_test_LDADD += $(top_srcdir)/src/config_parser.o
midi_state_test_LDADD += $(top_srcdir)/src/config_parameters.o
midi_state_test_LDADD += $(top_srcdir)/src/config_parameter.o
midi_state_test_LDADD += $(top_srcdir)/src/lexer.o
midi_state_test_LDADD += $(top_srcdir)/src/tokens.o
midi_state_test_LDADD += $(top_srcdir)/src/event.o

TESTS += midi_translator_test
check_PROGRAMS += midi_translator_test
midi_translator_test_SOURCES = midi_translator_test.cpp
midi_translator_test_LDADD = $(top_srcdir)/src/midi_translator.o
midi_translator_test_LDADD += $(top_srcdir)/src/midi_config.o
midi_translator_test_LDADD += $(top_srcdir)/src/config_directory.o
midi_translator_test_LDADD += $(top_srcdir)/src/config_parser.o
midi_translator_test_LDADD += $(top_srcdir)/src/config_parameters.o
midi_translator_test_LDADD += $(top_srcdir)/src/config_parameter.o
midi_translator_test_LDADD += $(top_srcdir)/src/lexer.o
midi_translator_test_LDADD += $(top_srcdir)/src/tokens.o
midi_translator_test_LDADD += $(top_srcdir)/src/event.o
midi_translator_test_LDADD += $(top_srcdir)/src/representation_config.o
midi_translator_test_LDADD += $(top_srcdir)/src/midi_min_max.o
midi_translator_test_LDADD += $(top_srcdir)/src/training_sequence.o

TESTS += training_results_test
check_PROGRAMS += training_results_test
training_results_test_SOURCES = training_results_test.cpp
training_results_test_LDADD = $(top_srcdir)/src/training_results.o
training_results_test_LDADD += $(top_srcdir)/src/lstm_config.o
training_results_test_LDADD += $(top_srcdir)/src/config_parser.o
training_results_test_LDADD += $(top_srcdir)/src/lstm_layer_config.o
training_results_test_LDADD += $(top_srcdir)/src/lexer.o
training_results_test_LDADD += $(top_srcdir)/src/tokens.o
training_results_test_LDADD += $(top_srcdir)/src/config_parameters.o
training_results_test_LDADD += $(top_srcdir)/src/config_parameter.o
training_results_test_LDADD += $(top_srcdir)/src/lstm_network.o
training_results_test_LDADD += $(top_srcdir)/src/lstm_unit_properties.o
training_results_test_LDADD += $(top_srcdir)/src/midi_min_max.o
training_results_test_LDADD += $(top_srcdir)/src/representation_config.o

TESTS += trainer_test
check_PROGRAMS += trainer_test
trainer_test_SOURCES = trainer_test.cpp
trainer_test_LDADD = $(top_srcdir)/src/trainer.o
trainer_test_LDADD += $(top_srcdir)/src/lstm_trainer.o
trainer_test_LDADD += $(top_srcdir)/src/event.o
trainer_test_LDADD += $(top_srcdir)/src/config_directory.o
trainer_test_LDADD += $(top_srcdir)/src/config_parser.o
trainer_test_LDADD += $(top_srcdir)/src/lstm_config.o
trainer_test_LDADD += $(top_srcdir)/src/lstm_architecture.o
trainer_test_LDADD += $(top_srcdir)/src/lstm_network.o
trainer_test_LDADD += $(top_srcdir)/src/midi_config.o
trainer_test_LDADD += $(top_srcdir)/src/representation_config.o
trainer_test_LDADD += $(top_srcdir)/src/training_event_stream.o
trainer_test_LDADD += $(top_srcdir)/src/tokens.o
trainer_test_LDADD += $(top_srcdir)/src/lexer.o
trainer_test_LDADD += $(top_srcdir)/src/config_parameter.o
trainer_test_LDADD += $(top_srcdir)/src/config_parameters.o
trainer_test_LDADD += $(top_srcdir)/src/lstm_layer_config.o
trainer_test_LDADD += $(top_srcdir)/src/lstm_unit_properties.o
trainer_test_LDADD += $(top_srcdir)/src/midi_translator.o
trainer_test_LDADD += $(top_srcdir)/src/midi_min_max.o
trainer_test_LDADD += $(top_srcdir)/src/training_sequence.o
trainer_test_LDADD += $(top_srcdir)/src/training_sequence_parser.o
trainer_test_LDADD += $(top_srcdir)/src/training_results.o
trainer_test_LDADD += $(top_srcdir)/src/training_config.o

TESTS += midi_file_reader_test
check_PROGRAMS += midi_file_reader_test
midi_file_reader_test_SOURCES = midi_file_reader_test.cpp
midi_file_reader_test_LDADD = $(top_srcdir)/src/midifile/MidiFile.o
midi_file_reader_test_LDADD += $(top_srcdir)/src/midifile/MidiMessage.o
midi_file_reader_test_LDADD += $(top_srcdir)/src/midifile/Binasc.o
midi_file_reader_test_LDADD += $(top_srcdir)/src/midifile/MidiEventList.o
midi_file_reader_test_LDADD += $(top_srcdir)/src/midifile/Options.o
midi_file_reader_test_LDADD += $(top_srcdir)/src/midifile/MidiEvent.o
midi_file_reader_test_LDADD += $(top_srcdir)/src/midi_file_reader.o
midi_file_reader_test_LDADD += $(top_srcdir)/src/event.o
