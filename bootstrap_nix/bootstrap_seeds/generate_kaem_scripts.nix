{ sources }:
let
  kaem-addition = ./kaem.c;
in
with sources;
rec {
  kaem_run = builtins.toFile "kaem.run" ''
    ${bootstrap-seeds}/POSIX/x86/hex0-seed ${bootstrap-seeds}/POSIX/x86/hex0_x86.hex0 hex0
    ./hex0 ${bootstrap-seeds}/POSIX/x86/kaem-minimal.hex0 kaem-0

    ./hex0 ${stage0-posix}/x86/hex1_x86.hex0 hex1
    ./hex0 ${stage0-posix}/x86/catm_x86.hex0 catm ## why hex0? maybe hex1 already?
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

    # Build kaem from C sources.
    ./M2 --architecture x86 \
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

    ./blood-elf-0 -f kaem.M1 -o kaem-footer.M1

    ./bin_M1 -f ${stage0-posix}/x86/x86_defs.M1 \
      -f ${stage0-posix}/x86/libc-core.M1 \
      -f kaem.M1 \
      -f kaem-footer.M1 \
      --LittleEndian \
      --architecture x86 \
      -o hold

    ./bin_hex2 -f ${stage0-posix}/x86/ELF-i386-debug.hex2 \
      -f hold \
      --LittleEndian \
      --architecture x86 \
      --BaseAddress 0x8048000 \
      -o bin_kaem \
      --exec_enable

    ./bin_kaem --verbose --strict -f ${kaem_full_run}
  '';
  kaem_full_run = builtins.toFile "kaem1.run" ''
    cd /build

    # Build blood-elf from C sources.
    ./M2 --architecture x86 \
      -f ${m2-planet}/test/common_x86/functions/exit.c \
      -f ${m2-planet}/test/common_x86/functions/file.c \
      -f ${m2-planet}/functions/file_print.c \
      -f ${mescc-tools}/functions/numerate_number.c \
      -f ${m2-planet}/test/common_x86/functions/malloc.c \
      -f ${m2-planet}/functions/calloc.c \
      -f ${m2-planet}/functions/match.c \
      -f ${m2-planet}/functions/require.c \
      -f ${m2-planet}/functions/in_set.c \
      -f ${mescc-tools}/blood-elf.c \
      --debug \
      -o blood-elf.M1

    ./blood-elf-0 -f blood-elf.M1 -o blood-elf-footer.M1

    ./bin_M1 -f ${stage0-posix}/x86/x86_defs.M1 \
      -f ${stage0-posix}/x86/libc-core.M1 \
      -f blood-elf.M1 \
      -f blood-elf-footer.M1 \
      --LittleEndian \
      --architecture x86 \
      -o hold

    ./bin_hex2 -f ${stage0-posix}/x86/ELF-i386-debug.hex2 \
      -f hold \
      --LittleEndian \
      --architecture x86 \
      --BaseAddress 0x8048000 \
      -o ./bin_blood-elf \
      --exec_enable

    # Build get_machine from C sources.
    ./M2 --architecture x86 \
      -f ${m2-planet}/test/common_x86/functions/exit.c \
      -f ${m2-planet}/test/common_x86/functions/file.c \
      -f ${m2-planet}/functions/file_print.c \
      -f ${m2-planet}/test/common_x86/functions/malloc.c \
      -f ${m2-planet}/functions/calloc.c \
      -f ${m2-planet}/test/common_x86/functions/uname.c \
      -f ${m2-planet}/functions/match.c \
      -f ${mescc-tools}/get_machine.c \
      --debug \
      -o get_machine.M1
    
    ./bin_blood-elf -f get_machine.M1 -o get_machine-footer.M1
    
    ./bin_M1 -f ${stage0-posix}/x86/x86_defs.M1 \
      -f ${stage0-posix}/x86/libc-core.M1 \
      -f get_machine.M1 \
      -f get_machine-footer.M1 \
      --LittleEndian \
      --architecture x86 \
      -o hold
    
    ./bin_hex2 -f ${stage0-posix}/x86/ELF-i386-debug.hex2 \
      -f hold \
      --LittleEndian \
      --architecture x86 \
      --BaseAddress 0x8048000 \
      -o ./bin_get_machine \
      --exec_enable

    # Build M2-planet from M2-planet.
    ./M2 --architecture x86 \
      -f ${m2-planet}/test/common_x86/functions/file.c \
      -f ${m2-planet}/test/common_x86/functions/malloc.c \
      -f ${m2-planet}/functions/calloc.c \
      -f ${m2-planet}/test/common_x86/functions/exit.c \
      -f ${m2-planet}/functions/match.c \
      -f ${m2-planet}/functions/in_set.c \
      -f ${m2-planet}/functions/numerate_number.c \
      -f ${m2-planet}/functions/file_print.c \
      -f ${m2-planet}/functions/number_pack.c \
      -f ${m2-planet}/functions/string.c \
      -f ${m2-planet}/functions/require.c \
      -f ${m2-planet}/functions/fixup.c \
      -f ${m2-planet}/cc.h \
      -f ${m2-planet}/cc_globals.c \
      -f ${m2-planet}/cc_reader.c \
      -f ${m2-planet}/cc_strings.c \
      -f ${m2-planet}/cc_types.c \
      -f ${m2-planet}/cc_core.c \
      -f ${m2-planet}/cc.c \
      --debug \
      -o M2.M1
    
    ./bin_blood-elf -f M2.M1 -o M2-footer.M1
    
    ./bin_M1 -f ${stage0-posix}/x86/x86_defs.M1 \
      -f ${stage0-posix}/x86/libc-core.M1 \
      -f M2.M1 \
      -f M2-footer.M1 \
      --LittleEndian \
      --architecture x86 \
      -o hold
    
    ./bin_hex2 -f ${stage0-posix}/x86/ELF-i386-debug.hex2 \
      -f hold \
      --LittleEndian \
      --architecture x86 \
      --BaseAddress 0x8048000 \
      -o ./bin_M2-Planet \
      --exec_enable

    # Build Mes-m2 from M2-Planet.
    ./bin_M2-Planet --debug --architecture x86 \
      -f ${mes-m2}/mes.h \
      -f ${m2-planet}/test/common_x86/functions/file.c \
      -f ${m2-planet}/test/common_x86/functions/exit.c \
      -f ${m2-planet}/test/common_x86/functions/malloc.c \
      -f ${m2-planet}/functions/calloc.c \
      -f ${mes-m2}/mes.c \
      -f ${mes-m2}/mes_cell.c \
      -f ${mes-m2}/mes_builtins.c \
      -f ${mes-m2}/mes_eval.c \
      -f ${mes-m2}/mes_print.c \
      -f ${mes-m2}/mes_read.c \
      -f ${mes-m2}/mes_tokenize.c \
      -f ${mes-m2}/mes_vector.c \
      -f ${mes-m2}/mes_list.c \
      -f ${mes-m2}/mes_string.c \
      -f ${mes-m2}/mes_keyword.c \
      -f ${mes-m2}/mes_record.c \
      -f ${mes-m2}/mes_init.c \
      -f ${mes-m2}/mes_macro.c \
      -f ${mes-m2}/mes_posix.c \
      -f ${mes-m2}/functions/numerate_number.c \
      -f ${mes-m2}/functions/match.c \
      -f ${mes-m2}/functions/in_set.c \
      -f ${mes-m2}/functions/file_print.c \
      -o mes.M1

    ./bin_blood-elf -f mes.M1 -o mes-footer.M1

    ./bin_M1 -f ${m2-planet}/test/common_x86/x86_defs.M1 \
      -f ${m2-planet}/test/common_x86/libc-core.M1 \
      -f mes.M1 \
      -f mes-footer.M1 \
      --LittleEndian \
      --architecture x86 \
      -o mes.hex2

    ./bin_hex2 -f ${m2-planet}/test/common_x86/ELF-i386-debug.hex2 \
      -f mes.hex2 \
      --LittleEndian \
      --architecture x86 \
      --BaseAddress 0x8048000 \
      -o ./bin_mes-m2 \
      --exec_enable

    mkdir ''${out}
    ./catm ''${out}/blood-elf-0 ./blood-elf-0
    chmod_x ''${out}/blood-elf-0

    ./catm ''${out}/M2 ./M2
    chmod_x ''${out}/M2


    mkdir ''${out}/bin
    ./catm ''${out}/bin/new_kaem ./bin_kaem
    chmod_x ''${out}/bin/new_kaem

    ./catm ''${out}/bin/M1 ./bin_M1
    chmod_x ''${out}/bin/M1

    ./catm ''${out}/bin/hex2 ./bin_hex2
    chmod_x ''${out}/bin/hex2

    ./catm ''${out}/bin/blood-elf ./bin_blood-elf
    chmod_x ''${out}/bin/blood-elf

    ./catm ''${out}/bin/get_machine ./bin_get_machine
    chmod_x ''${out}/bin/get_machine

    ./catm ''${out}/bin/M2-Planet ./bin_M2-Planet
    chmod_x ''${out}/bin/M2-Planet

    ./catm ''${out}/bin/M2-Planet ./bin_M2-Planet
    chmod_x ''${out}/bin/M2-Planet

    ./catm ''${out}/bin/mes-m2 ./bin_mes-m2
    chmod_x ''${out}/bin/mes-m2


  '';
}
