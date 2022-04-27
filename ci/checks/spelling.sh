#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

DICTIONARY="./ci/checks/dictionaries/spelling_corrections.txt"
SKIP=".git,\
./ci/checks/dictionaries/*,\
*package-lock.json,\
*node-packages.nix"

cspell -v --config=./ci/checks/dictionaries/cspell.json "**/*.*"
codespell -f -H -D=- -D="${DICTIONARY}" --skip="${SKIP}"
