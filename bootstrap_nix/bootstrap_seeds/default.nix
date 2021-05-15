let
  nixpkgs = import ../../ci/nix/pinned_nixpkgs.nix;
  pkgs = import nixpkgs { };
in
rec {
  bootstrap-seeds = builtins.fetchGit rec {
    name = "bootstrap-seeds";
    url = "https://github.com/oriansj/${name}.git";
    ref = "refs/heads/master";
    rev = "6d3fb087efe2d7cc7938cf2aff0265c6bfc86370";
  };
  m2-planet = builtins.fetchGit rec {
    name = "M2-Planet";
    url = "https://github.com/oriansj/${name}.git";
    ref = "refs/heads/master";
    rev = "cece07145c9f175767bf8bd83599be2f22e5bff4";
  };
  stage0-posix = pkgs.stdenv.mkDerivation rec {
    name = "stage0-posix";
    src = builtins.fetchGit {
      url = "https://github.com/oriansj/${name}.git";
      ref = "refs/heads/master";
      rev = "cb1c9d585690f6ead54a22caaa508ccd86cb863d";
    };
    kaem_run = ''
      ${bootstrap-seeds}/POSIX/x86/hex0-seed ${bootstrap-seeds}/POSIX/x86/hex0_x86.hex0 hex0
      hex0 ${bootstrap-seeds}/POSIX/x86/kaem-minimal.hex0 kaem-0

      hex0 ${src}/x86/hex1_x86.hex0 hex1
      hex0 ${src}/x86/catm_x86.hex0 catm ## why hex0? maybe hex1 already?
      hex1 ${src}/x86/hex2_x86.hex1 hex2-0

      catm hold ${src}/x86/ELF-i386.hex2 ${src}/x86/M0_x86.hex2
      hex2-0 hold M0

      # Building cc_x86
      M0 ${src}/x86/cc_x86.M1 temp1
      catm hold ${src}/x86/ELF-i386.hex2 temp1
      hex2-0 hold cc_x86

      # Building M2-Planet with cc_x86
      ${bootstrap-seeds}/POSIX/x86/hex0-seed ${bootstrap-seeds}/POSIX/x86/hex0_x86.hex0 hex0
    '';

    buildCommand = ''
      mkdir $out
      cp -r ${src}/* $out/
      echo "${kaem_run}" > $out/kaem.run
    '';
  };
  bootstrap-seeds-build = derivation rec {
    name = "bootstrap-seeds-build";
    system = builtins.currentSystem;
    outputs = [ "out" ];
    srcs = stage0-posix;
    builder = "${bootstrap-seeds}/POSIX/x86/kaem-optional-seed";
    args = [ "${srcs}/kaem.run" ];
  };

}
