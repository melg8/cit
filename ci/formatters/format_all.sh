#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

./ci/formatters/cmake_format.sh
./ci/formatters/nix_format.sh
./ci/formatters/clang_format.sh

echo "all formatters applied"
