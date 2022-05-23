# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

{ python39, callPackage }:
let
  packageOverrides = callPackage ./python-packages.nix { };
  python = python39.override { inherit packageOverrides; };
in
python.pkgs.codechecker
