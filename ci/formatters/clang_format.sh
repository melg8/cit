#!/usr/bin/env sh

set -e

find ./sources -name "*.h" -exec clang-format -i {} +
find ./sources -name "*.cpp" -exec clang-format -i {} +

echo "clang-format applied"
