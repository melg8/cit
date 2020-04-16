#!/bin/sh

set -e

git log --pretty="%h:%B" | \
cspell -v --config=./ci/checks/dictionaries/cspell.json stdin

find . -path ./.git -prune -o -printf "%f\n" | \
cspell -v --config=./ci/checks/dictionaries/cspell.json stdin

cspell -v --config=./ci/checks/dictionaries/cspell.json "**/*.*"