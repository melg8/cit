{ sources }:
let
  prepare-sources = list: builtins.concatStringsSep " "
    (builtins.map (x: "-f " + x) list) + " ";


in
with sources;
rec {
  buildWithM2 = with sources;
    { builder
    , elf
    , sources
    , name
    , M1
    , hex2
    , defsProvider ? "${stage0-posix}/x86"
    , outputPrefix
    }: ''
      ${builder} --debug --architecture x86 ''
    + prepare-sources sources + ''
      -o ${name}.M1

      ${elf} -f ${name}.M1 -o ${name}-footer.M1

      ${M1} -f ${defsProvider}/x86_defs.M1 \
       -f ${defsProvider}/libc-core.M1 \
       -f ${name}.M1 \
       -f ${name}-footer.M1 \
       --LittleEndian \
       --architecture x86 \
       -o ${name}.hex2

      ${hex2} -f ${defsProvider}/ELF-i386-debug.hex2 \
       -f ${name}.hex2 \
       --LittleEndian \
       --architecture x86 \
       --BaseAddress 0x8048000 \
       -o ${outputPrefix}${name} \
       --exec_enable
    '';

  buildWithWhichM2 = { M1, hex2, outputPrefix }:
    { builder
    , elf
    , sources
    , name
    , defsProvider ? "${stage0-posix}/x86"
    }:
    buildWithM2 {
      inherit builder elf sources name defsProvider M1 hex2 outputPrefix;
    };

  buildWithLocalM2 = buildWithWhichM2 {
    M1 = "./bin_M1";
    hex2 = "./bin_hex2";
    outputPrefix = "./bin_";
  };
  buildWithGlobalM2 = buildWithWhichM2 {
    M1 = "M1";
    hex2 = "hex2";
    outputPrefix = ''''${out}/bin/'';
  };

  build_kaem = binName: (''
    ${bootstrap-seeds}/POSIX/x86/hex0-seed ${bootstrap-seeds}/POSIX/x86/hex0_x86.hex0 hex0
    ./hex0 ${bootstrap-seeds}/POSIX/x86/kaem-minimal.hex0 kaem-0

    ./hex0 ${stage0-posix}/x86/hex1_x86.hex0 hex1
    ./hex0 ${stage0-posix}/x86/catm_x86.hex0 catm
    ./hex1 ${stage0-posix}/x86/hex2_x86.hex1 hex2-0

    ./catm hold ${stage0-posix}/x86/ELF-i386.hex2 ${stage0-posix}/x86/M0_x86.hex2
    ./hex2-0 hold M0

    # Building cc_x86
    ./M0 ${stage0-posix}/x86/cc_x86.M1 temp1
    ./catm hold ${stage0-posix}/x86/ELF-i386.hex2 temp1
    ./hex2-0 hold cc_x86

    # Build M2-Planet with cc_x86.
    ./catm hold ${m2-planet}/test/common_x86/functions/file.c \
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

    ./cc_x86 hold M2.M1
    ./catm hold ${stage0-posix}/x86/x86_defs.M1 ${stage0-posix}/x86/libc-core.M1 M2.M1
    ./M0 hold temp1
    ./catm hold ${stage0-posix}/x86/ELF-i386.hex2 temp1
    ./hex2-0 hold M2

    # Build blood-elf-0 with M2.
    ./M2 --architecture x86 \
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

    ./catm hold ${stage0-posix}/x86/x86_defs.M1 ${stage0-posix}/x86/libc-core.M1 blood-elf.M1
    ./M0 hold temp1
    ./catm hold ${stage0-posix}/x86/ELF-i386.hex2 temp1
    ./hex2-0 hold blood-elf-0

    # Build M1 from C sources.
    ./M2 --architecture x86 \
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

    ./blood-elf-0 -f M1-macro.M1 -o M1-macro-footer.M1
    ./catm hold ${stage0-posix}/x86/x86_defs.M1 ${stage0-posix}/x86/libc-core.M1 M1-macro.M1 M1-macro-footer.M1
    ./M0 hold temp1
    ./catm hold ${stage0-posix}/x86/ELF-i386-debug.hex2 temp1
    ./hex2-0 hold bin_M1

    # Build hex2 from C sources.
    ./M2 --architecture x86 \
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

    ./blood-elf-0 -f hex2_linker.M1 -o hex2_linker-footer.M1

    ./bin_M1 -f ${stage0-posix}/x86/x86_defs.M1 \
      -f ${stage0-posix}/x86/libc-core.M1 \
      -f hex2_linker.M1 \
      -f hex2_linker-footer.M1 \
      --LittleEndian \
      --architecture x86 \
      -o temp1

    ./catm hold ${stage0-posix}/x86/ELF-i386-debug.hex2 temp1
    ./hex2-0 hold bin_hex2

    ./catm HereStage1
  ''
  +
  buildWithLocalM2
    {
      name = binName;
      builder = "./M2";
      elf = "./blood-elf-0";
      sources = [
        "${m2-planet}/test/common_x86/functions/exit.c"
        "${m2-planet}/test/common_x86/functions/file.c"
        "${m2-planet}/functions/file_print.c"
        "${m2-planet}/test/common_x86/functions/malloc.c"
        "${m2-planet}/functions/calloc.c"
        "${m2-planet}/functions/match.c"
        "${m2-planet}/functions/in_set.c"
        "${m2-planet}/functions/require.c"
        "${mescc-tools}/functions/string.c"
        "${m2-planet}/functions/numerate_number.c"
        "${m2-planet}/test/common_x86/functions/fork.c"
        "${m2-planet}/test/common_x86/functions/execve.c"
        "${m2-planet}/test/common_x86/functions/chdir.c"
        "${m2-planet}/test/common_x86/functions/getcwd.c"
        "${mescc-tools}/Kaem/kaem.h"
        "${mescc-tools}/Kaem/variable.c"
        "${mescc-tools}/Kaem/kaem_globals.c"
        "${customKaem}"
      ];
    });
  kaemRun = builtins.toFile "kaem.run" (
    (build_kaem "kaem") +
    buildWithLocalM2
      {
        name = "blood-elf";
        builder = "./M2";
        elf = "./blood-elf-0";
        sources = [
          "${m2-planet}/test/common_x86/functions/exit.c"
          "${m2-planet}/test/common_x86/functions/file.c"
          "${m2-planet}/functions/file_print.c"
          "${mescc-tools}/functions/numerate_number.c"
          "${m2-planet}/test/common_x86/functions/malloc.c"
          "${m2-planet}/functions/calloc.c"
          "${m2-planet}/functions/match.c"
          "${m2-planet}/functions/require.c"
          "${m2-planet}/functions/in_set.c"
          "${mescc-tools}/blood-elf.c"
        ];
      }
    +
    buildWithLocalM2 {
      name = "get_machine";
      builder = "./M2";
      elf = "./bin_blood-elf";
      sources = [
        "${m2-planet}/test/common_x86/functions/exit.c"
        "${m2-planet}/test/common_x86/functions/file.c"
        "${m2-planet}/functions/file_print.c"
        "${m2-planet}/test/common_x86/functions/malloc.c"
        "${m2-planet}/functions/calloc.c"
        "${m2-planet}/test/common_x86/functions/uname.c"
        "${m2-planet}/functions/match.c"
        "${mescc-tools}/get_machine.c"
      ];
    }
    +
    buildWithLocalM2 {
      name = "M2-Planet";
      builder = "./M2";
      elf = "./bin_blood-elf";
      sources = [
        "${m2-planet}/test/common_x86/functions/file.c"
        "${m2-planet}/test/common_x86/functions/malloc.c"
        "${m2-planet}/functions/calloc.c"
        "${m2-planet}/test/common_x86/functions/exit.c"
        "${m2-planet}/functions/match.c"
        "${m2-planet}/functions/in_set.c"
        "${m2-planet}/functions/numerate_number.c"
        "${m2-planet}/functions/file_print.c"
        "${m2-planet}/functions/number_pack.c"
        "${m2-planet}/functions/string.c"
        "${m2-planet}/functions/require.c"
        "${m2-planet}/functions/fixup.c"
        "${m2-planet}/cc.h"
        "${m2-planet}/cc_globals.c"
        "${m2-planet}/cc_reader.c"
        "${m2-planet}/cc_strings.c"
        "${m2-planet}/cc_types.c"
        "${m2-planet}/cc_core.c"
        "${m2-planet}/cc.c"
      ];
    }
    +
    buildWithLocalM2 {
      name = "mes-m2";
      builder = "./bin_M2-Planet";
      elf = "./bin_blood-elf";
      sources = [
        "${mes-m2}/mes.h"
        "${m2-planet}/test/common_x86/functions/file.c"
        "${m2-planet}/test/common_x86/functions/exit.c"
        "${m2-planet}/test/common_x86/functions/malloc.c"
        "${m2-planet}/functions/calloc.c"
        "${mes-m2}/mes.c"
        "${mes-m2}/mes_cell.c"
        "${mes-m2}/mes_builtins.c"
        "${mes-m2}/mes_eval.c"
        "${mes-m2}/mes_print.c"
        "${mes-m2}/mes_read.c"
        "${mes-m2}/mes_tokenize.c"
        "${mes-m2}/mes_vector.c"
        "${mes-m2}/mes_list.c"
        "${mes-m2}/mes_string.c"
        "${mes-m2}/mes_keyword.c"
        "${mes-m2}/mes_record.c"
        "${mes-m2}/mes_init.c"
        "${mes-m2}/mes_macro.c"
        "${mes-m2}/mes_posix.c"
        "${mes-m2}/functions/numerate_number.c"
        "${mes-m2}/functions/match.c"
        "${mes-m2}/functions/in_set.c"
        "${mes-m2}/functions/file_print.c"
      ];
    }
    + ''
      ./bin_kaem --verbose --strict -f ${kaem_full_run_hook}
    ''
  );
  kaem_full_run_hook = builtins.toFile "kaem_full_run_hook.run" ''
    PATH=''${PATH}:''${PWD}
    ./bin_kaem --verbose --strict -f ${kaem_full_run}
  '';
  kaem_full_run = builtins.toFile "kaem_full_run.run" ''
    mkdir ''${out}
    catm ''${out}/blood-elf-0 ./blood-elf-0
    chmod_x ''${out}/blood-elf-0

    catm ''${out}/M2 ./M2
    chmod_x ''${out}/M2

    mkdir ''${out}/bin
    catm ''${out}/bin/catm ./catm
    chmod_x ''${out}/bin/catm

    catm ''${out}/bin/kaem ./bin_kaem
    chmod_x ''${out}/bin/kaem

    catm ''${out}/bin/M1 ./bin_M1
    chmod_x ''${out}/bin/M1

    catm ''${out}/bin/hex2 ./bin_hex2
    chmod_x ''${out}/bin/hex2

    catm ''${out}/bin/blood-elf ./bin_blood-elf
    chmod_x ''${out}/bin/blood-elf

    catm ''${out}/bin/get_machine ./bin_get_machine
    chmod_x ''${out}/bin/get_machine

    catm ''${out}/bin/M2-Planet ./bin_M2-Planet
    chmod_x ''${out}/bin/M2-Planet

    catm ''${out}/bin/M2-Planet ./bin_M2-Planet
    chmod_x ''${out}/bin/M2-Planet

    catm ''${out}/bin/mes-m2 ./bin_mes-m2
    chmod_x ''${out}/bin/mes-m2
  '';
}
