#!/bin/sh

set -e
ls-lint
find . -path ./.git -prune -o -print | \
cspell -v --config=./ci/checks/dictionaries/cspell.json stdin