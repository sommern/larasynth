/*
Copyright 2016 Nathan Sommer

This file is part of Larasynth.

Larasynth is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Larasynth is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Larasynth.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <string>

namespace larasynth {

static const std::string example_config =
R"(# Sections are denoted by names in square brackets. Lines that begin with a #
# are comments and are ignored by larasynth.

# The [midi] section specifies which controllers should be controlled by
# larasynth and what their default values should be. It can also be used to
# specify input and output MIDI ports.

[midi]
# Which controllers to control
controllers = 1, 2

# Default controller values are also specified as a comma separated list. Here
# controller 1 defaults to 0 and controller 2 defaults to 127
controller_defaults =
   1, 0,
   2, 127

# MIDI ports to use. These must be quoted strings. If they are not specified,
# you will be prompted to choose ports.
# recording_source_port = 
# performing_source_port = 
# performing_destination_port = 


# The [representation] section is where you specify how your training and
# performance data will be represented internally by larasynth.

[representation]
# The neural network has multiple outputs for each MIDI controller it is
# concerned with. This is customizable here.
controller_output_counts =
   1, 10,
   2, 10

# The update rate is the number of times per second the input is passed through
# the neural network and the controller values are re-calculated
update_rate = 75

# There are a number of ways that the information is encoded for input into the
# neural network. Commenting out or removing any that you don't need can speed
# up training.

input_features =
     "some note on",     # any note is currently playing
     "note struck",      # a note has just been struck
     "note released",    # a note has just been released
     "velocity",         # the velocity of the note that was just struck
     "interval"          # the interval between the last two struck notes


# The [lstm] section specifies the parameters for the LSTM neural network.

[lstm]
# The sizes of the hidden layers in the network. Often a single layer is
# sufficient
block_counts = 43

# Controls the rate at which the weights in the network are updated
learning_rate = 0.05
momentum = 0.8


[training]

# A random amount of silence is added to the beginning of each training example
# during training. This specifies the average and standard deviation of silence
# padding to add, in seconds.
mean_padding = 1.0
padding_stddev = 0.1

# Number of times each training example will be presented during one training
# epoch
example_repetitions = 2

# Number of times each training example will be presented during one validation
validation_example_repetitions = 5

# If this value is greater than zero, the tempo of each training example will
# be randomly adjusted each training epoch. 1.0 is the maximum adjustment
# factor and will adjust the tempo as dramatically as twice as fast or twice as
# slow.
tempo_adjustment_factor = 0.0

# If this value is greater than zero, some jitter will be randomly added to
# the duration between notes. 1.0 is the maximum jitter factor and will 
tempo_jitter_factor = 0.0

# The next 3 options are important as they must be lax if you need the
# network to keep learning past a difficult or impossible to learn portion of a
# sequence.

# Probability of resetting the training sequence when the network predicts
# the wrong controller values during training.
reset_probability = 0.5

# Specifies how wrong the network's prediction can be and still be considered
# correct. This is the sum of the squares of the error for each controller. So
# with 2 controllers a value of 5 would allow one controller to be off by 1
# (squared error of 1) and another off by 2 (squared error of 4).
squared_error_failure_tolerance = 8

# How many times in a row the network must be incorrect for a reset
consecutive_failures_for_reset = 5

# How many training epochs should be performed before the network validates
epoch_count_before_validating = 100

# Number of training epochs before training stops
max_epoch_count = 3000

# If the mean squared error for each time step reaches this threshold, then
# training will stop
mse_threshold = 0.0
)";

}
