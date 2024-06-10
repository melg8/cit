#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

COMPILER=$1
CONAN_COMPILER=$2
CONAN_COMPILER_VERSION=$3
CONAN_CPPSTD=$4
echo "compiler: " "${COMPILER}"
DIRECTORY=build_"${CONAN_COMPILER}"

mkdir -p "${DIRECTORY}"

cd "${DIRECTORY}"

conan profile detect -f
conan create ../ci/conan_recipes/range_v3/conanfile.py

conan install .. \
  --build missing \
  -of "${PWD}" \
  -s compiler=gcc -s compiler.version=13 -s compiler.cppstd=20 -s compiler.libcxx=libstdc++11 -s build_type=Debug
