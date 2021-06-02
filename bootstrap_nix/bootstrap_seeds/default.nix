let
  sources = import ./sources.nix;
  generatedKaemScripts = import ./generate_kaem_scripts.nix { inherit sources; };
  kaemRun = with sources; builtins.toFile "kaem.run" ''
    mkdir ''${out}
  '';
in
with sources;
rec  {
  MesM2WithTools = derivation rec {
    name = "MesM2WithTools";
    system = builtins.currentSystem;
    outputs = [ "out" ];
    srcs = generatedKaemScripts.kaemRun;
    builder = "${bootstrap-seeds}/POSIX/x86/kaem-optional-seed";
    args = [ "${srcs}" ];
  };
  MesCcToolsExtra =
    let
      src_path = "${live-bootstrap}/sysa/mescc-tools-extra/src";
      buildScript = with sources; with generatedKaemScripts;
        builtins.toFile "MesCcToolsExtra.run"
          (''
            mkdir ''${out}
            mkdir ''${out}/bin
          ''
          +
          buildWithGlobalM2 {
            name = "cp";
            builder = "M2-Planet";
            elf = "blood-elf";
            defsProvider = "${m2-planet}/test/common_x86";
            sources = [
              "${m2-planet}/test/common_x86/functions/file.c"
              "${m2-planet}/test/common_x86/functions/exit.c"
              "${m2-planet}/test/common_x86/functions/malloc.c"
              "${m2-planet}/test/common_x86/functions/getcwd.c"
              "${m2-planet}/test/common_x86/functions/chdir.c"
              "${src_path}/functions/string.c"
              "${src_path}/functions/file_print.c"
              "${src_path}/functions/match.c"
              "${src_path}/functions/require.c"
              "${m2-planet}/functions/calloc.c"
              "${src_path}/functions/in_set.c"
              "${src_path}/functions/numerate_number.c"
              "${src_path}/cp.c"
            ];
          }
          +
          buildWithGlobalM2 {
            name = "chmod";
            builder = "M2-Planet";
            elf = "blood-elf";
            defsProvider = "${m2-planet}/test/common_x86";
            sources = [
              "${m2-planet}/test/common_x86/functions/file.c"
              "${m2-planet}/test/common_x86/functions/exit.c"
              "${m2-planet}/test/common_x86/functions/malloc.c"
              "${m2-planet}/test/common_x86/functions/getcwd.c"
              "${m2-planet}/test/common_x86/functions/chdir.c"
              "${src_path}/functions/string.c"
              "${src_path}/functions/file_print.c"
              "${src_path}/functions/match.c"
              "${src_path}/functions/require.c"
              "${src_path}/functions/in_set.c"
              "${src_path}/functions/numerate_number.c"
              "${m2-planet}/functions/calloc.c"
              "${m2-planet}/test/common_x86/functions/stat.c"
              "${src_path}/chmod.c"
            ];
          }
          +
          buildWithGlobalM2 {
            name = "fletcher16";
            builder = "M2-Planet";
            elf = "blood-elf";
            defsProvider = "${m2-planet}/test/common_x86";
            sources = [
              "${m2-planet}/test/common_x86/functions/file.c"
              "${m2-planet}/test/common_x86/functions/exit.c"
              "${src_path}/functions/numerate_number.c"
              "${src_path}/functions/string.c"
              "${src_path}/functions/file_print.c"
              "${src_path}/functions/match.c"
              "${src_path}/functions/require.c"
              "${src_path}/functions/in_set.c"
              "${m2-planet}/functions/calloc.c"
              "${m2-planet}/test/common_x86/functions/malloc.c"
              "${src_path}/fletcher16.c"
            ];
          });
    in
    derivation rec {
      name = "MesCcToolsExtra";
      system = builtins.currentSystem;
      outputs = [ "out" ];
      srcs = buildScript;
      PATH = "${MesM2WithTools}:${MesM2WithTools}/bin";
      builder = "${MesM2WithTools}/bin/kaem";
      args = [ "--verbose" "--strict" "-f" "${srcs}" ];
    };
  MesWip213BuildByM2 =
    let
      buildScript = with sources; with generatedKaemScripts;
        builtins.toFile "MesWip213BuildByM2.run"
          (''
            mkdir ''${out}
            mkdir ''${out}/bin
          '' +
          buildWithGlobalM2 {
            name = "mes";
            builder = "M2-Planet";
            elf = "blood-elf";
            defsProvider = "${mes-wip-2_13}/lib/m2/x86";
            sources = [
              "${mes-wip-2_13}/include/m2/lib.h"
              "${mes-wip-2_13}/lib/linux/x86-mes-m2/crt1.c"
              "${mes-wip-2_13}/lib/linux/x86-mes-m2/mini.c"
              "${mes-wip-2_13}/lib/mes/globals.c"
              "${mes-wip-2_13}/lib/m2/cast.c"
              "${mes-wip-2_13}/lib/m2/exit.c"
              "${mes-wip-2_13}/lib/mes/mini-write.c"
              "${mes-wip-2_13}/lib/linux/x86-mes-m2/syscall.c"
              "${mes-wip-2_13}/include/linux/x86/syscall.h"
              "${mes-wip-2_13}/lib/linux/brk.c"
              "${mes-wip-2_13}/lib/stdlib/malloc.c"
              "${mes-wip-2_13}/lib/string/memset.c"
              "${mes-wip-2_13}/lib/m2/read.c"
              "${mes-wip-2_13}/lib/mes/fdgetc.c"
              "${mes-wip-2_13}/lib/stdio/getchar.c"
              "${mes-wip-2_13}/lib/stdio/putchar.c"
              "${mes-wip-2_13}/lib/m2/open.c"
              "${mes-wip-2_13}/lib/m2/mes_open.c"
              "${mes-wip-2_13}/lib/string/strlen.c"
              "${mes-wip-2_13}/lib/mes/eputs.c"
              "${mes-wip-2_13}/lib/mes/fdputc.c"
              "${mes-wip-2_13}/lib/mes/eputc.c"
              "${mes-wip-2_13}/include/mes/mes.h"
              "${mes-wip-2_13}/include/mes/builtins.h"
              "${mes-wip-2_13}/include/mes/constants.h"
              "${mes-wip-2_13}/include/mes/symbols.h"
              "${mes-wip-2_13}/lib/mes/__assert_fail.c"
              "${mes-wip-2_13}/lib/mes/assert_msg.c"
              "${mes-wip-2_13}/lib/mes/fdputc.c"
              "${mes-wip-2_13}/lib/string/strncmp.c"
              "${mes-wip-2_13}/lib/posix/getenv.c"
              "${mes-wip-2_13}/lib/mes/fdputs.c"
              "${mes-wip-2_13}/lib/mes/ntoab.c"
              "${mes-wip-2_13}/lib/ctype/isdigit.c"
              "${mes-wip-2_13}/lib/ctype/isxdigit.c"
              "${mes-wip-2_13}/lib/ctype/isspace.c"
              "${mes-wip-2_13}/lib/ctype/isnumber.c"
              "${mes-wip-2_13}/lib/mes/abtol.c"
              "${mes-wip-2_13}/lib/stdlib/atoi.c"
              "${mes-wip-2_13}/lib/string/memcpy.c"
              "${mes-wip-2_13}/lib/stdlib/free.c"
              "${mes-wip-2_13}/lib/stdlib/realloc.c"
              "${mes-wip-2_13}/lib/string/strcpy.c"
              "${mes-wip-2_13}/lib/mes/itoa.c"
              "${mes-wip-2_13}/lib/mes/ltoa.c"
              "${mes-wip-2_13}/lib/mes/fdungetc.c"
              "${mes-wip-2_13}/lib/posix/setenv.c"
              "${mes-wip-2_13}/lib/linux/access.c"
              "${mes-wip-2_13}/lib/m2/chmod.c"
              "${mes-wip-2_13}/lib/linux/ioctl3.c"
              "${mes-wip-2_13}/lib/m2/isatty.c"
              "${mes-wip-2_13}/lib/linux/fork.c"
              "${mes-wip-2_13}/lib/m2/execve.c"
              "${mes-wip-2_13}/lib/m2/execv.c"
              "${mes-wip-2_13}/lib/linux/waitpid.c"
              "${mes-wip-2_13}/lib/linux/gettimeofday.c"
              "${mes-wip-2_13}/lib/m2/clock_gettime.c"
              "${mes-wip-2_13}/lib/m2/time.c"
              "${mes-wip-2_13}/lib/linux/_getcwd.c"
              "${mes-wip-2_13}/lib/m2/getcwd.c"
              "${mes-wip-2_13}/lib/linux/dup.c"
              "${mes-wip-2_13}/lib/linux/dup2.c"
              "${mes-wip-2_13}/lib/string/strcmp.c"
              "${mes-wip-2_13}/lib/string/memcmp.c"
              "${mes-wip-2_13}/lib/linux/unlink.c"
              "${mes-wip-2_13}/src/builtins.c"
              "${mes-wip-2_13}/src/core.c"
              "${mes-wip-2_13}/src/display.c"
              "${mes-wip-2_13}/src/eval-apply.c"
              "${mes-wip-2_13}/src/gc.c"
              "${mes-wip-2_13}/src/hash.c"
              "${mes-wip-2_13}/src/lib.c"
              "${mes-wip-2_13}/src/m2.c"
              "${mes-wip-2_13}/src/math.c"
              "${mes-wip-2_13}/src/mes.c"
              "${mes-wip-2_13}/src/module.c"
              "${mes-wip-2_13}/src/posix.c"
              "${mes-wip-2_13}/src/reader.c"
              "${mes-wip-2_13}/src/stack.c"
              "${mes-wip-2_13}/src/string.c"
              "${mes-wip-2_13}/src/struct.c"
              "${mes-wip-2_13}/src/symbol.c"
              "${mes-wip-2_13}/src/vector.c"
            ];
            m1Sources = [
              "${mes-wip-2_13}/lib/m2/x86/x86_defs.M1"
              "${mes-wip-2_13}/lib/x86-mes/x86.M1"
              "${mes-wip-2_13}/lib/linux/x86-mes-m2/crt1.M1"
            ];
            baseAddress = "0x1000000";
          });
    in
    derivation rec {
      name = "MesWip213BuildByM2";
      system = builtins.currentSystem;
      outputs = [ "out" ];
      srcs = buildScript;
      PATH = "${MesM2WithTools}:${MesM2WithTools}/bin";
      builder = "${MesM2WithTools}/bin/kaem";
      args = [ "--verbose" "--strict" "-f" "${srcs}" ];
    };
  kaemEnvTest = derivation rec {
    name = "kaemEnvTest";
    system = builtins.currentSystem;
    outputs = [ "out" ];
    srcs = kaemRun;
    PATH = "${MesM2WithTools}:${MesM2WithTools}/bin";
    builder = "${MesM2WithTools}/bin/kaem";
    args = [ "--verbose" "--strict" "-f" "${srcs}" ];
  };
  kaemEnvTest1 = derivation rec {
    name = "kaemEnvTest1";
    system = builtins.currentSystem;
    outputs = [ "out" ];
    srcs = kaemRun;
    PATH = "${MesM2WithTools}:${MesM2WithTools}/bin:${kaemEnvTest}/bin";
    builder = kaemEnvTest.drvAttrs.builder;
    args = [ "--verbose" "--strict" "-f" "${srcs}" ];
  };
  kaemEnvTest2 = derivation rec {
    name = "kaemEnvTest2";
    system = builtins.currentSystem;
    outputs = [ "out" ];
    srcs = kaemRun;
    PATH = "${MesM2WithTools}:${MesM2WithTools}/bin:${kaemEnvTest}/bin";
    builder = kaemEnvTest1.drvAttrs.builder;
    args = kaemEnvTest1.args;
    allowedRequisites = [ kaemEnvTest1 kaemEnvTest MesM2WithTools ];
  };
}
