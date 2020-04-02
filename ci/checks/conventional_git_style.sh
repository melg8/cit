#!/bin/sh

set -e
conform enforce
commitlint -t HEAD
gitlint --commits HEAD
gitleaks --threads=$(nproc) --verbose --pretty --repo-path=.
git log --pretty="%h:%B" | \
cspell -v --config=./ci/checks/dictionaries/cspell.json stdin
