#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

apply_to_file_names() {
	find . -type d \
		\( \
		-path ./.git -o \
		-path ./build -o \
		-path ./build_gcc -o \
		-path ./build_clang \) \
		-prune -o -print |
		"$@"
}

ls-lint

apply_to_file_names cspell -v --config=./ci/checks/dictionaries/cspell.json stdin
