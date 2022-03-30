#!/usr/bin/env sh

set -e

COMPILER=$1
echo "compiler: " "${COMPILER}"
DIRECTORY=build_"${COMPILER}"

mkdir -p "${DIRECTORY}"
cd "${DIRECTORY}"
cmake .. -D CMAKE_CXX_COMPILER="${COMPILER}"
cmake --build . -j "$(nproc)"
