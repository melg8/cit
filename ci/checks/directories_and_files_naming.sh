#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

ls-lint
find . -path ./.git -prune -o -print | \
cspell -v --config=./ci/checks/dictionaries/cspell.json stdin