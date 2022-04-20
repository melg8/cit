#!/usr/bin/env sh

set -e

COMPILER=$1
CONAN_COMPILER=$2
CONAN_COMPILER_VERSION=$3
echo "compiler: " "${COMPILER}"
DIRECTORY=build_"${CONAN_COMPILER}"

mkdir -p "${DIRECTORY}"
cd "${DIRECTORY}"

conan install .. \
-s compiler="${CONAN_COMPILER}" \
-s compiler.version="${CONAN_COMPILER_VERSION}"
