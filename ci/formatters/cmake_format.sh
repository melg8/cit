#!/bin/sh

set -e

ARGUMENTS="-c=./.cmake_format.yml CMakeLists.txt ./**/CMakeLists.txt ./**/*.cmake"

# shellcheck disable=SC2086
cmake-format --in-place ${ARGUMENTS} &&
echo "cmake-format applied"
