#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

DEFAULT_WORKSPACE="$(pwd)"
export DEFAULT_WORKSPACE

MEGALINTER_FLAVOR=cit
export MEGALINTER_FLAVOR

command time megalinter --flavor

echo "All checks are passed."
