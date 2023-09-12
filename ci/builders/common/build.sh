#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

COMPILER=$1
CONAN_COMPILER=$2
CONAN_COMPILER_VERSION=$3

./ci/builders/common/cmake_setup.sh \
	"${COMPILER}" "${CONAN_COMPILER}" "${CONAN_COMPILER_VERSION}"

DIRECTORY=build_"${CONAN_COMPILER}"
cd "${DIRECTORY}"

cmake .. -G Ninja \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_CXX_COMPILER="${COMPILER}" \
	-DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"

cmake --build . -j "$(nproc)"

mkdir -p ../report

ctest --verbose |
	sed 's/[0-9]\+\: //g' >../report/ctest_logs_"${COMPILER}".txt

export GCOV=gcov-13

grcov . \
	-s .. \
	--ignore "/nix/store/*" \
	--ignore "*/.conan/*" \
	-t lcov >cit_"${CONAN_COMPILER}"_test_coverage.info
