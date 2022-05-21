# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

let
  rev = "48037fd90426e44e4bf03e6479e88a11453b9b66";
in
builtins.fetchTarball {
  url = "https://github.com/NixOS/nixpkgs/archive/${rev}.tar.gz";
  sha256 = "sha256:0b2y5spv8v6ysn3kiyj11bwdr25jc27gd0xjid3hv29qb6z2z230";
}
