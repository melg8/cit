{ pkgs, stdenv, nodejs }:
let
  package = pkgs.callPackage ./default.nix { };
  args = package.args // {
    preRebuild = ''
      substituteInPlace node_modules/dockerfile_lint/lib/linter-utils.js \
      --replace "_.contains" "_.includes"
    '';
  };
in
(package.shell.override args).nodeDependencies
