#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

find ./sources -name "*.h" -exec clang-format --dry-run --Werror {} +
find ./sources -name "*.cpp" -exec clang-format --dry-run --Werror {} +

 # Disabled checks of cpplint:
 # build/c++11 - to allow system_error usage.
 # runtime/references - to allow += operator overload with reference.
 # build/header_guard - to allow easy move of headers between folders without
 # changes in header guards.
cpplint \
--recursive \
--linelength=80 \
--includeorder=standardcfirst \
--filter=\
-build/c++11,\
-build/header_guard,\
-runtime/references \
--root=./sources \
./sources

./ci/builders/common/cmake_setup.sh g++ gcc 11

DIRECTORY="./build_gcc"
cd "${DIRECTORY}"

cmake .. -G Ninja \
-DCMAKE_CXX_COMPILER=g++ \
-DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"

cd ..

cppcheck \
--cppcheck-build-dir="${DIRECTORY}" \
--project="${DIRECTORY}"/compile_commands.json \
--library=./.config/cppcheck/doctest.cfg \
--suppress=*:/home/user/.conan/data/* \
--suppress=*:/nix/store/* \
--error-exitcode=1 \
--enable=all \
--inline-suppr \
--inconclusive \
--suppress=unusedFunction \
--suppress=unusedStructMember \
--suppress=unmatchedSuppression \
--suppress=missingIncludeSystem

./ci/builders/common/cmake_setup.sh clang++ clang 13

DIRECTORY="./build_clang"
cd "${DIRECTORY}"

cmake .. -G Ninja \
-DCMAKE_CXX_COMPILER=clang++ \
-DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"

cd ..

find ./sources -name "*.cpp" -exec \
run-clang-tidy -header-filter='^(u(i[^_]|[^i])|[^u])+$' -j"$(nproc --all)" \
-p="${DIRECTORY}" {} +