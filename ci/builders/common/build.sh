#!/usr/bin/env bash

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e
set -o pipefail

COMPILER=$1
CONAN_COMPILER=$2
CONAN_COMPILER_VERSION=$3
CONAN_CPPSTD=$4

./ci/builders/common/cmake_setup.sh \
  "${COMPILER}" "${CONAN_COMPILER}" "${CONAN_COMPILER_VERSION}" "${CONAN_CPPSTD}"

DIRECTORY=build_"${CONAN_COMPILER}"
cd "${DIRECTORY}"

cmake .. --preset "cit_"${CONAN_COMPILER}"_debug"

echo "Building..."
cmake --build . -j "$(nproc)" | camomilla -c"../.camomilla.json"

mkdir -p ../report

ninjatracing ./.ninja_log > ../report/"${COMPILER}"_trace.json

echo "Running tests..."
ctest --verbose |
  sed 's/[0-9]\+\: //g' >../report/ctest_logs_"${COMPILER}".txt

export GCOV=gcov-13

echo "Creating coverage report..."
grcov . \
  -s .. \
  --ignore "/nix/store/*" \
  --ignore "*/.conan/*" \
  -t lcov >cit_"${CONAN_COMPILER}"_test_coverage.info

echo "All done."
