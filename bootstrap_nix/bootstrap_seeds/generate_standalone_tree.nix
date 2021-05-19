let
  nixpkgs = import ../../ci/nix/pinned_nixpkgs.nix;
  pkgs = import nixpkgs { };
  generate_kaem_scripts = import ./generate_kaem_scripts.nix {
    sources = builtins.listToAttrs (map (x: { name = x; value = x; })
      (builtins.attrNames (import ./sources.nix)));
  };
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
    depsBuildBuild = [ nukeReferences ];

    kaem_sources = lib.lists.zipListsWith (a: b: a + ":" + b)
      (builtins.attrNames (import ./sources.nix))
      (builtins.map (builtins.getAttr "outPath") (builtins.attrValues (import ./sources.nix)));

    installPhase = ''
      mkdir -p $out

      for source in $kaem_sources; do
        directory=''${source%:*}
        outPath=''${source#*:}
        echo "mkdir: $directory from sources out: $outPath"

        mkdir -p $directory
        cp -r $outPath/* $directory/
        nuke-refs $directory

        unset directory
        unset outPath
      done

      cp -r * $out/
      cp ${generate_kaem_scripts.kaem_run} $out/kaem.run
      sed -e 's,/nix/store/.*kaem1\.run,kaem1\.run,' -i $out/kaem.run
      cp ${generate_kaem_scripts.kaem_full_run} $out/kaem1.run
      sed -e 's,cd /build,out=\./kaem_full,' -i $out/kaem1.run

      nuke-refs $out
      nuke-refs $out/kaem.run
      nuke-refs $out/kaem1.run
    '';
  };
  build-standalone-tree = stdenv.mkDerivation {
    pname = "build-standalone-tree";
    version = "0.1";

    srcs = [ standalone-tree ];
    installPhase = ''
        mkdir build_tree
        cp -r ${standalone-tree}/* build_tree/
        cd build_tree
        ./bootstrap-seeds/POSIX/x86/kaem-optional-seed ./kaem.run
        mkdir -p $out/bin
        cp ./kaem_full/bin/* $out/bin/
    '';
    dontUnpack = true;
    dontPatch = true;
    dontConfigure = true;
    dontBuild = true;
    dontFixup = true;
  };
}
