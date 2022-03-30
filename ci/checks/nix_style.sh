#!/usr/bin/env sh

set -e
export LANG=C.UTF-8

find . -type f -name "*.nix" -exec nixpkgs-fmt --check {} +

find . -type f -name '*.nix' ! -name '*node-*.nix' -exec nix-linter {} +
