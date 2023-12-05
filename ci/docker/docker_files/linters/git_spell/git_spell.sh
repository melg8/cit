#!/usr/bin/env bash

# SPDX-FileCopyrightText: © 2023 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e
git log --pretty="%B" |
  cspell -v --config=./ci/checks/dictionaries/cspell.json stdin://git_log_temp.md
echo "git-spell OK"
