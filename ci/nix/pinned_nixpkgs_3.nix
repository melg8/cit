# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

let
  rev = "f91ee3065de91a3531329a674a45ddcb3467a650";
in
builtins.fetchTarball {
  url = "https://github.com/NixOS/nixpkgs/archive/${rev}.tar.gz";
  sha256 = "sha256:1jgcgllp8pxjz3q6r2yswsvlg96wlcnvdpgi71k7inibffqhq0z9";
}
