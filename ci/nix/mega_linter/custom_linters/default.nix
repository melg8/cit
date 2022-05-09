# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

{ stdenv }:

stdenv.mkDerivation rec {
  pname = "mega_linter_custom_files";
  version = "0.0.7";

  src = ./.;

  phases = "unpackPhase installPhase";

  installPhase = ''
    rm ./default.nix
    cp -R ./ $out
  '';

}
