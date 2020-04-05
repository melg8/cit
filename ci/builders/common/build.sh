#!/bin/sh

set -e

COMPILER=$1
echo "compiler: " ${COMPILER}

mkdir build
cd build
${COMPILER} ../temporary.cc
./a.out