#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

./ci/builders/gcc/build.sh

genhtml \
  --output-directory coverage \
  --demangle-cpp \
  --num-spaces 2 \
  --sort \
  --title "cit test coverage" \
  --function-coverage \
  --branch-coverage \
  --legend ./build_gcc/cit_gcc_test_coverage.info
