#!/bin/sh
rm -rf build
mkdir -p build
cd build
conan install .. --build=missing -s build_type=Release -s compiler.libcxx=libstdc++11
CC=gcc-12 CXX=g++-12 cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON