#!/usr/bin/env sh

set -e

./ci/formatters/cmake_format.sh
./ci/formatters/nix_format.sh

echo "all formatters applied"