#!/bin/sh

set -e
command time -v conform enforce
command time -v commitlint -t HEAD
command time -v gitlint --commits HEAD
command time -v  gitleaks --threads="$(nproc)" --verbose --pretty --repo-path=.
command time -v  git log --pretty="%h:%B" | \
cspell -v --config=./ci/checks/dictionaries/cspell.json stdin \
command time -v git-cop -p
