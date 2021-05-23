let
  nixpkgs = import ../../ci/nix/pinned_nixpkgs.nix;
  pkgs = import nixpkgs { };
  base = import ./default.nix;
  pkg = base.kaem-env-test-2;
in
with pkgs; rec {
  standalone-tree = stdenv.mkDerivation rec {
    pname = "kaem_build_tree";
    version = "0.1";

    dontUnpack = true;
    dontPatch = true;
    dontConfigure = true;
    dontBuild = true;
    dontFixup = true;
    depsBuildBuild = [ pkgs.nix ];

    closureInfo = pkgs.closureInfo { rootPaths = pkg; };

    installPhase = ''
      mkdir -p $out
      mkdir -p /nix/var
      ${nix}/bin/nix-store --load-db < ${closureInfo}/registration
      ${nix}/bin/nix-store -qR pkg.drvPath > $out/store_paths.txt
    '';
  };
}
