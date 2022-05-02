#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

nix build -f ./ci/nix/docker_build_2.nix

docker load < ./result

docker container run --rm -it -v "$(pwd)":/home/user/work nix:latest reuse lint | \
not grep "can't start new thread" && echo "PASSED!" || echo "FAILED!"