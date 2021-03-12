#!/usr/bin/env sh

set -e

git diff --check HEAD^
git-sizer | grep -q "No problems"