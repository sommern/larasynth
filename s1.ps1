# Create a new directory named 'build'
mkdir build

# Change directory to 'build'
cd build

# Run CMake configuration using MinGW Makefiles generator
cmake .. -G "MinGW Makefiles"

# Build the project using 'mingw32-make' with 16 parallel jobs
mingw32-make -j 16

# Change directory to 'src'
cd src

# Run ./lara.exe t config and type 'y'
echo 'y' | ./lara.exe aaa config

# Run ./lara.exe t import with the specified MIDI file path
./lara.exe aaa import "A:\Downloads\larasynth_tutorial.mid"

# Run ./lara.exe t train
./lara.exe aaa train
