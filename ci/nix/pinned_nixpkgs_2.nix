# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

let
  rev = "2dea0f4c2d6e4603f54b2c56c22367e77869490c";
in
builtins.fetchTarball {
  url = "https://github.com/NixOS/nixpkgs/archive/${rev}.tar.gz";
  sha256 = "sha256:1sazdjffsjhg4qcvx18bymd8hkfiq8mpzfd23cx6xnwjzps7fkzy";
}
