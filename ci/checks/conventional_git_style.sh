#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

# Setup default branch on server side.
git config init.defaultBranch origin/main

command time -v conform enforce
command time -v commitlint -t HEAD
command time -v gitlint --commits HEAD
command time -v  gitleaks --verbose detect .
command time -v  git log --pretty="%B" | \
cspell -v --config=./ci/checks/dictionaries/cspell.json stdin

if git show --summary | grep -q ^Merge: ; then
    echo "is merge commit not apply git-lint"
else
  echo "regular commit"
  command time -v git-lint -a
fi
