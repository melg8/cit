# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

{ writeShellScriptBin, nixpkgs-fmt }:
writeShellScriptBin "nixpkgs-fmt-wrapper" ''
  ${nixpkgs-fmt}/bin/nixpkgs-fmt $@
''
