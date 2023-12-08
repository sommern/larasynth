mkdir build

cd build

cmake .. -DCMAKE_BUILD_TYPE=Release

msbuild .\larasynth.sln -maxcpucount:8

cd src/Debug

echo 'y' | ./lara.exe t config

./lara.exe t import "C:\Users\aarip\Downloads\larasynth_tutorial.mid"

./lara.exe t train
