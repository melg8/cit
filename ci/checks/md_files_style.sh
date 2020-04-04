#!/bin/sh

set -e
remark . -f \
--use remark-preset-lint-consistent \
--use remark-preset-lint-markdown-style-guide \
--use remark-preset-lint-recommended

mdl .

ignore=license.md

find "$(pwd)" -name "*.md" ! -iname "${ignore}" \
-exec textlint {} +