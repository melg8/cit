rec {
  nixpkgs = import ../../ci/nix/pinned_nixpkgs.nix;
  pkgs = import nixpkgs { };
  base = import ./default.nix;
  pkg = base.kaem-env-test-2;
  value = builtins.foldl' (s: name: s + "${name}=${toString pkg.drvAttrs.${name}}\n") "" (builtins.attrNames pkg.drvAttrs);
  result = builtins.trace (builtins.getContext value) (builtins.getContext value);
}
