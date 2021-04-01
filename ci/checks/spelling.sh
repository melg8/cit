#!/usr/bin/env sh

DICTIONARY="./ci/checks/dictionaries/spelling_corrections.txt"
SKIP=".git,\
./ci/checks/dictionaries/*,\
./ci/docker/docker_files/scripts/cleanups/permissions.sh,\
*package-lock.json,\
*node-packages.nix"

set -e
cspell -v --config=./ci/checks/dictionaries/cspell.json "**/*.*"
codespell -f -H -D=- -D="${DICTIONARY}" --skip="${SKIP}"
