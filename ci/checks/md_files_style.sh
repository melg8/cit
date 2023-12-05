#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

remark . -o --rc-path rc.yaml --frail

mdl .

ignore=license.md

find "$(pwd)" -name "*.md" ! -iname "${ignore}" \
  -exec textlint --experimental --parallel {} +
