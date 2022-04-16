#!/usr/bin/env sh

set -e

find . -type f -name "*.nix" -exec nixpkgs-fmt {} +
echo "nix-format applied"
