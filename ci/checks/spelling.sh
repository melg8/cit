#!/bin/sh

DICTIONARY="./ci/checks/dictionaries/spelling_corrections.txt"
SKIP=".git,./ci/checks/dictionaries/*"

set -e
cspell -v --config=./ci/checks/dictionaries/cspell.json "**/*.*"
codespell -f -H -D=- -D=${DICTIONARY} --skip=${SKIP}