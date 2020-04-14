#!/bin/sh

set -e

PRESETS="/usr/local/lib/node_modules"

remark . -f \
--use "${PRESETS}"/remark-preset-lint-consistent \
--use "${PRESETS}"/remark-preset-lint-markdown-style-guide \
--use "${PRESETS}"/remark-preset-lint-recommended

mdl .

ignore=license.md

find "$(pwd)" -name "*.md" ! -iname "${ignore}" \
-exec textlint {} +