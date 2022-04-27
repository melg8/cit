#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

remark . -r rc.yaml -f

mdl .

ignore=license.md

find "$(pwd)" -name "*.md" ! -iname "${ignore}" \
-exec textlint --experimental --parallel {} +