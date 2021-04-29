#!/usr/bin/env bash

set -e

nix build -f ./ci/nix/docker_build.nix \
             --enforce-determinism \
             --keep-failed

ls -la .
sha256sum ./result