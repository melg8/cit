#!/bin/sh

set -e

mkdir build
cd build
g++ ../temporary.cc
./a.out