{ sources }:
let
  kaem-addition = ./kaem.c;
in
with sources;
rec {
  kaem_run = builtins.toFile "kaem.run" ''
    ${bootstrap-seeds}/POSIX/x86/hex0-seed ${bootstrap-seeds}/POSIX/x86/hex0_x86.hex0 hex0
    hex0 ${bootstrap-seeds}/POSIX/x86/kaem-minimal.hex0 kaem-0

    hex0 ${stage0-posix}/x86/hex1_x86.hex0 hex1
    hex0 ${stage0-posix}/x86/catm_x86.hex0 catm ## why hex0? maybe hex1 already?
    hex1 ${stage0-posix}/x86/hex2_x86.hex1 hex2-0

    catm hold ${stage0-posix}/x86/ELF-i386.hex2 ${stage0-posix}/x86/M0_x86.hex2
    hex2-0 hold M0

    # Building cc_x86
    M0 ${stage0-posix}/x86/cc_x86.M1 temp1
    catm hold ${stage0-posix}/x86/ELF-i386.hex2 temp1
    hex2-0 hold cc_x86

    # Build M2-Planet with cc_x86.
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
    catm hold ${stage0-posix}/x86/x86_defs.M1 ${stage0-posix}/x86/libc-core.M1 M2.M1
    M0 hold temp1
    catm hold ${stage0-posix}/x86/ELF-i386.hex2 temp1
    hex2-0 hold M2

    # Build blood-elf-0 with M2.
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

    catm hold ${stage0-posix}/x86/x86_defs.M1 ${stage0-posix}/x86/libc-core.M1 blood-elf.M1
    M0 hold temp1
    catm hold ${stage0-posix}/x86/ELF-i386.hex2 temp1
    hex2-0 hold blood-elf-0

    # Build M1 from c sources.
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
    catm hold ${stage0-posix}/x86/x86_defs.M1 ${stage0-posix}/x86/libc-core.M1 M1-macro.M1 M1-macro-footer.M1
    M0 hold temp1
    catm hold ${stage0-posix}/x86/ELF-i386-debug.hex2 temp1
    hex2-0 hold bin_M1

    # Build hex2 from c sources.
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

    bin_M1 -f ${stage0-posix}/x86/x86_defs.M1 \
      -f ${stage0-posix}/x86/libc-core.M1 \
      -f hex2_linker.M1 \
      -f hex2_linker-footer.M1 \
      --LittleEndian \
      --architecture x86 \
      -o temp1

    catm hold ${stage0-posix}/x86/ELF-i386-debug.hex2 temp1
    hex2-0 hold bin_hex2

    # Build kaem from c sources.
    M2 --architecture x86 \
      -f ${m2-planet}/test/common_x86/functions/exit.c \
      -f ${m2-planet}/test/common_x86/functions/file.c \
      -f ${m2-planet}/functions/file_print.c \
      -f ${m2-planet}/test/common_x86/functions/malloc.c \
      -f ${m2-planet}/functions/calloc.c \
      -f ${m2-planet}/functions/match.c \
      -f ${m2-planet}/functions/in_set.c \
      -f ${m2-planet}/functions/require.c \
      -f ${mescc-tools}/functions/string.c \
      -f ${m2-planet}/functions/numerate_number.c \
      -f ${m2-planet}/test/common_x86/functions/fork.c \
      -f ${m2-planet}/test/common_x86/functions/execve.c \
      -f ${m2-planet}/test/common_x86/functions/chdir.c \
      -f ${m2-planet}/test/common_x86/functions/getcwd.c \
      -f ${mescc-tools}/Kaem/kaem.h \
      -f ${mescc-tools}/Kaem/variable.c \
      -f ${mescc-tools}/Kaem/kaem_globals.c \
      -f ${kaem-addition} \
      --debug \
      -o kaem.M1

    blood-elf-0 -f kaem.M1 -o kaem-footer.M1

    bin_M1 -f ${stage0-posix}/x86/x86_defs.M1 \
      -f ${stage0-posix}/x86/libc-core.M1 \
      -f kaem.M1 \
      -f kaem-footer.M1 \
      --LittleEndian \
      --architecture x86 \
      -o hold

    bin_hex2 -f ${stage0-posix}/x86/ELF-i386-debug.hex2 \
      -f hold \
      --LittleEndian \
      --architecture x86 \
      --BaseAddress 0x8048000 \
      -o bin_kaem \
      --exec_enable

    bin_kaem --verbose --strict -f ${kaem_full_run}
  '';
  kaem_full_run = builtins.toFile "kaem1.run" ''
    mkdir ''${out}
    cd /build

    ./catm ''${out}/new_kaem ./bin_kaem
    chmod_x ''${out}/new_kaem

    ./catm ''${out}/blood-elf-0 ./blood-elf-0
    chmod_x ''${out}/blood-elf-0

    ./catm ''${out}/M2 ./M2
    chmod_x ''${out}/M2

    ./catm ''${out}/hex2 ./bin_hex2
    chmod_x ''${out}/hex2
  '';
}
