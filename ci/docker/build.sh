#!/usr/bin/env bash

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

nix --extra-experimental-features nix-command \
	build -f ./ci/nix/docker_build.nix \
	--keep-failed

ls -la .
sha256sum ./result
