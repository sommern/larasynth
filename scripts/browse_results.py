#!/usr/bin/env python3
#
# Copyright 2016 Nathan Sommer
# 
# This file is part of Larasynth.
# 
# Larasynth is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Larasynth is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Larasynth.  If not, see <http://www.gnu.org/licenses/>.


import sys
import os
import matplotlib.pyplot as plt
import json

plt.ion()


class Result:
    def __init__(self, json_filename):
        self.json_filename = json_filename

        with open(self.json_filename) as f:
            data = json.load(f)

        print(self.json_filename)

        self.epoch = data['epoch']
        self.mse = data['mse']

        print('epoch:', self.epoch)
        print('MSE:', self.mse)

        self.ctrls = data['ctrls']
        self.ctrl_count = len(self.ctrls)

        self.cell_count = data['cell_count']

        self.sample_count = data['sample_count']

        self.all_targets = {}
        self.all_outputs = {}
        self.all_cell_states = [[] for _ in range(self.cell_count)]

        for ctrl in self.ctrls:
            self.all_targets[ctrl] = []
            self.all_outputs[ctrl] = []

        json_targets = data['targets']
        json_outputs = data['outputs']
        json_cell_states = data['cell_states']

        i = 0
        while i < len(json_targets):
            for ctrl in self.ctrls:
                self.all_targets[ctrl].append(json_targets[i])
                self.all_outputs[ctrl].append(json_outputs[i])

                i += 1

        i = 0
        for x in range(len(json_cell_states) // self.cell_count):
            for cell in range(self.cell_count):
                self.all_cell_states[cell].append(json_cell_states[i])
                i += 1


class Results:
    def __init__(self, dir_or_dirs):
        if type(dir_or_dirs) == str:
            self.directories = [dir_or_dirs]
        else:
            self.directories = dir_or_dirs

        self.json_filenames = []

        for directory in self.directories:
            for filename in os.listdir(directory):
                if filename.endswith('.json'):
                    path = os.path.join(directory, filename)
                    self.json_filenames.append(path)

        self.results = []

        for filename in self.json_filenames:
            self.results.append(Result(filename))

    def get_mse_epoch_filename_result_list(self):
        mse_epoch_filename_list = []

        for result in self.results:
            mse_epoch_filename_list.append((result.mse, result.epoch,
                                           result.json_filename, result))

        return sorted(mse_epoch_filename_list)


def plot_vectors(title, x, y, label=None):
    plt.figure(title)
    plt.plot(x, y, label=label)
    plt.draw()


def browse_result(result: Result):
    all_targets = result.all_targets
    all_outputs = result.all_outputs

    figure_name = 'Targets and Outputs'

    plt.figure(figure_name)
    plt.clf()

    for ctrl in result.ctrls:
        targets = all_targets[ctrl]
        outputs = all_outputs[ctrl]

        target_label = 'ctrl {0} target'.format(ctrl)
        output_label = 'ctrl {0} output'.format(ctrl)

        plot_vectors(figure_name, list(range(len(targets))), targets,
                     target_label)
        plot_vectors(figure_name, list(range(len(outputs))), outputs,
                     output_label)

    #plt.legend()
    plt.suptitle('epoch ' + str(result.epoch) + ', MSE = ' + str(result.mse))

    figure_name = 'Cell States'

    plt.figure(figure_name)
    plt.clf()
    for cell_states in result.all_cell_states:
        plot_vectors(figure_name, list(range(len(cell_states))), cell_states)

    input()
    plt.close()


def pick_result(results):
    choices = results.get_mse_epoch_filename_result_list()

    print('Results:')
    print()

    for i, (mse, epoch, json_filename, result) in enumerate(choices):
        print(i, ':', str(mse)[:6], json_filename, epoch)

    print()

    choice = None

    while not choice:
        try:
            input_choice = int(input('Select a result: '))
            choice = choices[input_choice]
        except (ValueError, IndexError):
            print('Invalid choice')
        except EOFError:
            break

    return choice[3]


def main():
    if len(sys.argv) == 1:
        sys.exit('expected a list of directories')

    results = Results(sys.argv[1:])

    result = pick_result(results)

    if result is not None:
        browse_result(result)


if __name__ == "__main__":
    main()
