# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

let
  rev = "4f77ea639305f1de0a14d9d41eef83313360638c";
in
builtins.fetchTarball {
  url = "https://github.com/NixOS/nixpkgs/archive/${rev}.tar.gz";
  sha256 = "sha256:13487jd9165hhravrzs8sm1j2swpcnv84jdiiiza4ma206vjji1w";
}
