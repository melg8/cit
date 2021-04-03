#!/usr/bin/env sh

set -e

MODULES_PATH=/usr/lib/node_modules

remark . -f \
--use ${MODULES_PATH}/remark-preset-lint-consistent \
--use ${MODULES_PATH}/remark-preset-lint-markdown-style-guide \
--use ${MODULES_PATH}/remark-preset-lint-recommended

mdl .

ignore=license.md

find "$(pwd)" -name "*.md" ! -iname "${ignore}" \
-exec textlint {} +