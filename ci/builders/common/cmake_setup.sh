#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

COMPILER=$1
CONAN_COMPILER=$2
CONAN_COMPILER_VERSION=$3
echo "compiler: " "${COMPILER}"
DIRECTORY=build_"${CONAN_COMPILER}"

mkdir -p "${DIRECTORY}"

cd "${DIRECTORY}"

conan profile detect -f
conan create ../ci/conan_recipes/range-v3/conanfile.py

conan install .. \
	--build missing \
	-of "${PWD}" \
	-s compiler="${CONAN_COMPILER}" \
	-s compiler.version="${CONAN_COMPILER_VERSION}"
