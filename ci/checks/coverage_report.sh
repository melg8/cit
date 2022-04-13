#!/usr/bin/env sh

set -e

./ci/builders/gcc/build.sh

lcov --directory build_gcc --capture --output-file cit_test_coverage.info

genhtml --output-directory coverage --demangle-cpp --num-spaces 2\
 --sort --title "cit test coverage" --function-coverage\
 --branch-coverage --legend cit_test_coverage.info
