#!/usr/bin/env sh

set -e

remark . -r rc.yaml -f

mdl .

ignore=license.md

find "$(pwd)" -name "*.md" ! -iname "${ignore}" \
-exec textlint --experimental --parallel {} +