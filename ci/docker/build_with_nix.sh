#!/usr/bin/env bash

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

nix build -f ./ci/nix/docker_build.nix \
             --enforce-determinism \
             --keep-failed

ls -la .
sha256sum ./result