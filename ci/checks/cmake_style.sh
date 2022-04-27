#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

ARGUMENTS="-c=./.cmake_format.yml CMakeLists.txt ./**/CMakeLists.txt ./**/*.cmake"

# shellcheck disable=SC2086
cmake-lint ${ARGUMENTS}

# shellcheck disable=SC2086
cmake-format --check ${ARGUMENTS} ||
(echo "Failure: please run cmake-format." && exit 1)

echo "Cmake style passed."