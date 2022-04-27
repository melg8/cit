#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

find ./sources -name "*.h" -exec clang-format -i {} +
find ./sources -name "*.cpp" -exec clang-format -i {} +

echo "clang-format applied"
