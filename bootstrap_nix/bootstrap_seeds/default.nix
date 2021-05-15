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
    rev = "a5bc08b23fbe74f7f7eb2842eb8468be4d031d3f"; # Release 1.7.0
  };
  mescc-tools = builtins.fetchGit rec {
    name = "mescc-tools";
    url = "https://github.com/oriansj/${name}.git";
    ref = "refs/heads/master";
    rev = "5768b2a79036f34b9bd420ab4801ad7dca15dff8"; # Compatible with m2-planet 1.7.0
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

      # Build M2-Planet with cc_x86
      catm hold ${m2-planet}/test/common_x86/functions/file.c \
        ${m2-planet}/test/common_x86/functions/malloc.c \
        ${m2-planet}/functions/calloc.c \
        ${m2-planet}/test/common_x86/functions/exit.c \
        ${m2-planet}/functions/match.c \
        ${m2-planet}/functions/in_set.c \
        ${m2-planet}/functions/numerate_number.c \
        ${m2-planet}/functions/file_print.c \
        ${m2-planet}/functions/number_pack.c \
        ${m2-planet}/functions/string.c \
        ${m2-planet}/functions/require.c \
        ${m2-planet}/functions/fixup.c \
        ${m2-planet}/cc.h \
        ${m2-planet}/cc_globals.c \
        ${m2-planet}/cc_reader.c \
        ${m2-planet}/cc_strings.c \
        ${m2-planet}/cc_types.c \
        ${m2-planet}/cc_core.c \
        ${m2-planet}/cc.c

      cc_x86 hold M2.M1
      catm hold ${src}/x86/x86_defs.M1 ${src}/x86/libc-core.M1 M2.M1
      M0 hold temp1
      catm hold ${src}/x86/ELF-i386.hex2 temp1
      hex2-0 hold M2

      # Build blood-elf-0 with M2
      M2 --architecture x86 \
        -f ${m2-planet}/test/common_x86/functions/exit.c \
        -f ${m2-planet}/test/common_x86/functions/file.c \
        -f ${m2-planet}/functions/file_print.c \
        -f ${mescc-tools}/functions/numerate_number.c \
        -f ${m2-planet}/test/common_x86/functions/malloc.c \
        -f ${m2-planet}/functions/calloc.c \
        -f ${m2-planet}/functions/match.c \
        -f ${mescc-tools}/blood-elf.c \
        -f ${m2-planet}/functions/require.c \
        -f ${m2-planet}/functions/in_set.c \
        -o blood-elf.M1

      catm hold ${src}/x86/x86_defs.M1 ${src}/x86/libc-core.M1 blood-elf.M1
      M0 hold temp1
      catm hold ${src}/x86/ELF-i386.hex2 temp1
      hex2-0 hold blood-elf-0

      # Build M1 from c sources
      M2 --architecture x86 \
         -f ${m2-planet}/test/common_x86/functions/exit.c \
         -f ${m2-planet}/test/common_x86/functions/file.c \
         -f ${m2-planet}/functions/file_print.c \
         -f ${m2-planet}/test/common_x86/functions/malloc.c \
         -f ${m2-planet}/functions/calloc.c \
         -f ${m2-planet}/functions/match.c \
         -f ${m2-planet}/functions/in_set.c \
         -f ${m2-planet}/functions/numerate_number.c \
         -f ${m2-planet}/functions/string.c \
         -f ${m2-planet}/functions/require.c \
         -f ${mescc-tools}/M1-macro.c \
         --debug \
         -o M1-macro.M1

      blood-elf-0 -f M1-macro.M1 -o M1-macro-footer.M1
      catm hold ${src}/x86/x86_defs.M1 ${src}/x86/libc-core.M1 M1-macro.M1 M1-macro-footer.M1
      M0 hold temp1
      catm hold ${src}/x86/ELF-i386-debug.hex2 temp1
      hex2-0 hold bin_M1

      # Build hex2 from c sources
      M2 --architecture x86 \
          -f ${m2-planet}/test/common_x86/functions/exit.c \
          -f ${m2-planet}/test/common_x86/functions/file.c \
          -f ${m2-planet}/functions/file_print.c \
          -f ${m2-planet}/test/common_x86/functions/malloc.c \
          -f ${m2-planet}/functions/calloc.c \
          -f ${m2-planet}/functions/match.c \
          -f ${m2-planet}/functions/require.c \
          -f ${m2-planet}/functions/in_set.c \
          -f ${m2-planet}/functions/numerate_number.c \
          -f ${m2-planet}/test/common_x86/functions/stat.c \
          -f ${mescc-tools}/hex2_linker.c \
          --debug \
          -o hex2_linker.M1
      blood-elf-0 -f hex2_linker.M1 -o hex2_linker-footer.M1
      bin_M1 -f ${src}/x86/x86_defs.M1 \
      	-f ${src}/x86/libc-core.M1 \
      	-f hex2_linker.M1 \
      	-f hex2_linker-footer.M1 \
      	--LittleEndian \
      	--architecture x86 \
      	-o temp1

      catm hold ${src}/x86/ELF-i386-debug.hex2 temp1
      hex2-0 hold bin_hex2

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
