#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

DEFAULT_WORKSPACE="$(pwd)"
export DEFAULT_WORKSPACE

MEGALINTER_FLAVOR=cit
export MEGALINTER_FLAVOR

if megalinter --flavor; then
	echo "All checks are passed."
else
	echo "To view the results in a browser run:
    > firefox $(pwd)/report/errors/index.html"
	exit 1
fi
