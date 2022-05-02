# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

let
  rev = "e10da1c7f542515b609f8dfbcf788f3d85b14936";
in
builtins.fetchTarball {
  url = "https://github.com/NixOS/nixpkgs/archive/${rev}.tar.gz";
  sha256 = "sha256:1if304v4i4lm217kp9f11f241kl3drbix3d0f08vgd6g43pv5mhq";
}
