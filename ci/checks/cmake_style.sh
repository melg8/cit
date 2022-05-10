#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

ARGUMENTS="-c=./.cmake_format.yml"

apply_to_files() {
	find . -type d \
		\( -path ./build_gcc -o -path ./build_clang -o -path ./build \) -prune \
		-o \( -name "*.cmake" -o -name "CMakeLists.txt" \) -print \
		-exec "$@" {} +
}

apply_to_files cmake-lint ${ARGUMENTS}

apply_to_files cmake-format --check ${ARGUMENTS} ||
	(echo "Failure: please run cmake-format." && exit 1)

echo "Cmake style passed."
