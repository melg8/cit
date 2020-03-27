#!/bin/sh

DICTIONARY="./ci/checks/dictionaries/spelling_corrections.txt"
SKIP=".git,./ci/checks/dictionaries/*"

set -e
cspell "sources/**/*.*"
codespell -f -H -D=- -D=${DICTIONARY} --skip=${SKIP}