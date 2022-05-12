#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

find . -type f -name "*.nix" -exec nixpkgs-fmt --check {} +

find . -type f -name '*.nix' ! -name '*node-*.nix' -exec nix-linter {} +
