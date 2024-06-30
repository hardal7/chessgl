#!/bin/bash

rm -rf build
rm lib/lib*
rm -rf lib/glfw/build

cd ./lib/glfw
cmake -B build -D BUILD_SHARED_LIBS=ON
cd ./build/src
make
mv lib* ../../..
rm -rf ../../build
cd ../../../glad
gcc -c glad.c -fPIC
gcc -shared -o libglad.so glad.o
mv libglad.so ../
rm glad.o
cd ../../
