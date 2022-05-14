# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

let
  rev = "710fed5a2483f945b14f4a58af2cd3676b42d8c8";
in
builtins.fetchTarball {
  url = "https://github.com/NixOS/nixpkgs/archive/${rev}.tar.gz";
  sha256 = "sha256:1xhbkgb9rzh2b0rbyhcygvc6216g9qbqyjkkgrhwfclsx06sfach";
}
