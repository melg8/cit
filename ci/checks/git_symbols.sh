#!/usr/bin/env sh

set -e

git diff --check HEAD^ 1> /dev/null
git-sizer | grep -q "No problems"