# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

let
  nixpkgs = import ./ci/nix/pinned_nixpkgs.nix;
  nixpkgs2 = import ./ci/nix/pinned_nixpkgs_2.nix;
  nixpkgs3 = import ./ci/nix/pinned_nixpkgs_3.nix;
  pkgs = import nixpkgs { };
  pkgs2 = import nixpkgs2 { };
  pkgs3 = import nixpkgs3 { };
in
pkgs2.mkShell.override { stdenv = pkgs3.gcc13Stdenv; } rec {
  buildInputs = import ./ci/nix/tools.nix { inherit pkgs; inherit pkgs2; inherit pkgs3;};
  fetchScripts = buildInputs;
  shellHook = ''source <(just --completions=bash);for input in ${builtins.toString buildInputs}; do if [[ -e "$input/lib/node_modules" ]] ; then export NODE_PATH="$input/lib/node_modules:$NODE_PATH";fi;done'';
}
