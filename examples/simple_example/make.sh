#!/usr/bin/env bash

mkdir build
g++ -c ./main.cpp -o ./build/main.o -I ./../../src -std=c++17
g++ -c ./../../src/tupp.cpp -o ./build/tupp.o -I ./../../src -std=c++17
g++ ./build/tupp.o ./build/main.o -o ./build/main.exe -std=c++17
