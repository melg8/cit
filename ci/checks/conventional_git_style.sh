#!/usr/bin/env sh

set -e

# Setup default branch on server side.
git config init.defaultBranch origin/main


command time -v conform enforce
command time -v commitlint -t HEAD
command time -v gitlint --commits HEAD
command time -v  gitleaks --verbose detect .
command time -v  git log --pretty="%B" | \
cspell -v --config=./ci/checks/dictionaries/cspell.json stdin
command time -v git-lint -a
