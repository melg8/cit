#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

DEFAULT_WORKSPACE="$(pwd)"
export DEFAULT_WORKSPACE

if [ -z "${PYTHONPATH}" ]; then
env PYTHONPATH="$(find /nix/store \
                -name 'site-packages' \
                -exec echo {} + | sed -e 's/ /:/g')" \
python -m megalinter.run
else
python -m megalinter.run
fi

echo "All checks are passed."