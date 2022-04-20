#!/usr/bin/env sh

set -e

COMPILER=$1
CONAN_COMPILER=$2
CONAN_COMPILER_VERSION=$3

./ci/builders/common/cmake_setup.sh \
"${COMPILER}" "${CONAN_COMPILER}" "${CONAN_COMPILER_VERSION}"

DIRECTORY=build_"${CONAN_COMPILER}"
cd "${DIRECTORY}"

cmake .. -G Ninja \
-DCMAKE_CXX_COMPILER="${COMPILER}" \
-DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"

cmake --build . -j "$(nproc)"
ctest

grcov . \
-s .. \
--ignore "/nix/store/*" \
--ignore "*/.conan/*" \
-t lcov > cit_"${CONAN_COMPILER}"_test_coverage.info
