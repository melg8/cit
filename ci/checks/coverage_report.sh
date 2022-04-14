#!/usr/bin/env sh

set -e

./ci/builders/gcc/build.sh

genhtml --output-directory coverage --demangle-cpp --num-spaces 2\
 --sort --title "cit test coverage" --function-coverage\
 --branch-coverage --legend ./build_gcc/cit_gcc_test_coverage.info
