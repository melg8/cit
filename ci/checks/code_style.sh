#!/usr/bin/env sh

set -e

find ./sources -name "*.h" -exec clang-format --dry-run --Werror {} +
find ./sources -name "*.cpp" -exec clang-format --dry-run --Werror {} +
