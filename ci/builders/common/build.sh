#!/usr/bin/env sh

set -e

COMPILER=$1
echo "compiler: " "${COMPILER}"
DIRECTORY=build_"${COMPILER}"

mkdir -p "${DIRECTORY}"
cd "${DIRECTORY}"
cmake .. -G Ninja -D CMAKE_CXX_COMPILER="${COMPILER}"
cmake --build . -j "$(nproc)"
ctest
