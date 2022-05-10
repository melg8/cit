#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

DICTIONARY="./ci/checks/dictionaries/spelling_corrections.txt"
SKIP=".git,\
./ci/checks/dictionaries/*,\
*package-lock.json,\
*CMakeLists.txt.user,\
*node-packages.nix"

apply_to_files() {
	find . -type d \
		\( \
		-path ./.git -o \
		-path ./build -o \
		-path ./build_gcc -o \
		-path ./build_clang \) \
		-prune -o -type f -print \
		-exec "$@" {} +
}

apply_to_files cspell -v --config=./ci/checks/dictionaries/cspell.json
codespell -f -H -D=- -D="${DICTIONARY}" --skip="${SKIP}"
