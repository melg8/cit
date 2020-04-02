#!/bin/sh

set -e
ls-lint
find . -path ./.git -prune -o -printf "%f\n" | \
cspell -v --config=./ci/checks/dictionaries/cspell.json stdin