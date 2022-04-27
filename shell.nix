# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

let
  nixpkgs = import ./ci/nix/pinned_nixpkgs.nix;
  pkgs = import nixpkgs { };
in
pkgs.mkShell.override { stdenv = pkgs.gcc11Stdenv; } rec {
  buildInputs = import ./ci/nix/tools.nix { inherit pkgs; };

  shellHook = ''
    for input in ${builtins.toString buildInputs}; do
       if [[ -e "$input/lib/node_modules" ]] ; then
           export NODE_PATH="$input/lib/node_modules:$NODE_PATH";
       fi
    done
  '';
}
