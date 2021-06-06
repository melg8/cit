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
  MesBootstrapWip213BuildByM2 =
    let
      buildScript = with sources; with generatedKaemScripts;
        builtins.toFile "MesBootstrapWip213BuildByM2.run"
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
          }
          + ''
            ''${out}/bin/mes -c "(display 'Hello,M2-mes!) (newline)"
          ''
          );
    in
    derivation rec {
      name = "MesBootstrapWip213BuildByM2";
      system = builtins.currentSystem;
      outputs = [ "out" ];
      srcs = buildScript;
      PATH = "${MesM2WithTools}:${MesM2WithTools}/bin";
      MES_PREFIX = "${mes-wip-2_13}";
      GUILE_LOAD_PATH = "${mes-wip-2_13}/module:${mes-wip-2_13}/mes/module";
      libdir = "${mes-wip-2_13}/lib";
      builder = "${MesM2WithTools}/bin/kaem";
      args = [ "--verbose" "--strict" "-f" "${srcs}" ];
    };

  MesWip213BuildByM2 =
    let
      copyFile = from: to: subPath: ''
        catm ${to}/${subPath} ${from}/${subPath}
      '';
      copyFiles = { from, to, subPaths }: builtins.concatStringsSep "\n"
        (map (copyFile from to) subPaths);
      buildWithMes = file: ''
        mes \
          --no-auto-compile \
          -e main ${live-bootstrap}/sysa/mes/files/mescc.scm \
          -- -c -D HAVE_CONFIG_H=1 \
          -I . \
          -I ${mes-wip-2_13}/include \
          -I ${mes-wip-2_13}/\
          ${file}
      '';
      replaceExtensionTo = to: file: builtins.replaceStrings [ ".c" ] [ to ] file;
      compiledFileNames = extension: files:
        builtins.concatStringsSep " "
          (map (replaceExtensionTo extension)
            (map builtins.baseNameOf files));
      createSingleUnit = path: extension: files:
        "catm ${path} " + compiledFileNames extension files;
      buildLibWithMes = { name, files, outputExtension ? ".a" }:
        let
          outputFolder = ''''${out}/lib/x86-mes'';
        in
        ''
          ${builtins.concatStringsSep "\n" (map buildWithMes files)}
          ${createSingleUnit ''${outputFolder}/${name}${outputExtension}'' ".o" files}
          ${createSingleUnit ''${outputFolder}/${name}.s'' ".s" files}
        '';
      buildScript = with sources;
        builtins.toFile "MesWip213BuildByM2.run"
          (
            ''
              mkdir ''${out}
              mkdir ''${out}/bin
              mkdir ''${out}/lib
              mkdir ''${out}/lib/x86-mes

              mkdir mes
              catm ./mes/config.h ${live-bootstrap}/sysa/mes/files/config.h

            '' + buildLibWithMes {
              name = "crt1";
              outputExtension = ".o";
              files = [ "${mes-wip-2_13}/lib/linux/x86-mes-mescc/crt1.c" ];
            }
            + buildLibWithMes {
              name = "libc-mini";
              files = [
                "${mes-wip-2_13}/lib/mes/eputs.c"
                "${mes-wip-2_13}/lib/mes/oputs.c"
                "${mes-wip-2_13}/lib/mes/globals.c"
                "${mes-wip-2_13}/lib/stdlib/exit.c"
                "${mes-wip-2_13}/lib/linux/x86-mes-mescc/_exit.c"
                "${mes-wip-2_13}/lib/linux/x86-mes-mescc/_write.c"
                "${mes-wip-2_13}/lib/stdlib/puts.c"
                "${mes-wip-2_13}/lib/string/strlen.c"
                "${mes-wip-2_13}/lib/mes/mini-write.c"
              ];
            }
            + buildLibWithMes {
              name = "libmescc";
              files = [
                "${mes-wip-2_13}/lib/mes/globals.c"
                "${mes-wip-2_13}/lib/linux/x86-mes-mescc/syscall-internal.c"
              ];
            }
            + buildLibWithMes {
              name = "libc";
              files = [
                "${mes-wip-2_13}/lib/mes/eputs.c"
                "${mes-wip-2_13}/lib/mes/oputs.c"
                "${mes-wip-2_13}/lib/mes/globals.c"
                "${mes-wip-2_13}/lib/stdlib/exit.c"
                "${mes-wip-2_13}/lib/linux/x86-mes-mescc/_exit.c"
                "${mes-wip-2_13}/lib/linux/x86-mes-mescc/_write.c"
                "${mes-wip-2_13}/lib/stdlib/puts.c"
                "${mes-wip-2_13}/lib/string/strlen.c"
                "${mes-wip-2_13}/lib/ctype/isnumber.c"
                "${mes-wip-2_13}/lib/mes/abtol.c"
                "${mes-wip-2_13}/lib/mes/cast.c"
                "${mes-wip-2_13}/lib/mes/eputc.c"
                "${mes-wip-2_13}/lib/mes/fdgetc.c"
                "${mes-wip-2_13}/lib/mes/fdputc.c"
                "${mes-wip-2_13}/lib/mes/fdputs.c"
                "${mes-wip-2_13}/lib/mes/fdungetc.c"
                "${mes-wip-2_13}/lib/mes/itoa.c"
                "${mes-wip-2_13}/lib/mes/ltoa.c"
                "${mes-wip-2_13}/lib/mes/ltoab.c"
                "${mes-wip-2_13}/lib/mes/mes_open.c"
                "${mes-wip-2_13}/lib/mes/ntoab.c"
                "${mes-wip-2_13}/lib/mes/oputc.c"
                "${mes-wip-2_13}/lib/mes/ultoa.c"
                "${mes-wip-2_13}/lib/mes/utoa.c"
                "${mes-wip-2_13}/lib/ctype/isdigit.c"
                "${mes-wip-2_13}/lib/ctype/isspace.c"
                "${mes-wip-2_13}/lib/ctype/isxdigit.c"
                "${mes-wip-2_13}/lib/mes/assert_msg.c"
                "${mes-wip-2_13}/lib/posix/write.c"
                "${mes-wip-2_13}/lib/stdlib/atoi.c"
                "${mes-wip-2_13}/lib/linux/lseek.c"
                "${mes-wip-2_13}/lib/mes/__assert_fail.c"
                "${mes-wip-2_13}/lib/mes/__buffered_read.c"
                "${mes-wip-2_13}/lib/mes/__mes_debug.c"
                "${mes-wip-2_13}/lib/posix/execv.c"
                "${mes-wip-2_13}/lib/posix/getcwd.c"
                "${mes-wip-2_13}/lib/posix/getenv.c"
                "${mes-wip-2_13}/lib/posix/isatty.c"
                "${mes-wip-2_13}/lib/posix/open.c"
                "${mes-wip-2_13}/lib/posix/buffered-read.c"
                "${mes-wip-2_13}/lib/posix/setenv.c"
                "${mes-wip-2_13}/lib/posix/wait.c"
                "${mes-wip-2_13}/lib/stdio/fgetc.c"
                "${mes-wip-2_13}/lib/stdio/fputc.c"
                "${mes-wip-2_13}/lib/stdio/fputs.c"
                "${mes-wip-2_13}/lib/stdio/getc.c"
                "${mes-wip-2_13}/lib/stdio/getchar.c"
                "${mes-wip-2_13}/lib/stdio/putc.c"
                "${mes-wip-2_13}/lib/stdio/putchar.c"
                "${mes-wip-2_13}/lib/stdio/ungetc.c"
                "${mes-wip-2_13}/lib/stdlib/free.c"
                "${mes-wip-2_13}/lib/stdlib/malloc.c"
                "${mes-wip-2_13}/lib/stdlib/realloc.c"
                "${mes-wip-2_13}/lib/string/memchr.c"
                "${mes-wip-2_13}/lib/string/memcmp.c"
                "${mes-wip-2_13}/lib/string/memcpy.c"
                "${mes-wip-2_13}/lib/string/memmove.c"
                "${mes-wip-2_13}/lib/string/memset.c"
                "${mes-wip-2_13}/lib/string/strcmp.c"
                "${mes-wip-2_13}/lib/string/strcpy.c"
                "${mes-wip-2_13}/lib/string/strncmp.c"
                "${mes-wip-2_13}/lib/posix/raise.c"
                "${mes-wip-2_13}/lib/linux/access.c"
                "${mes-wip-2_13}/lib/linux/brk.c"
                "${mes-wip-2_13}/lib/linux/chmod.c"
                "${mes-wip-2_13}/lib/linux/clock_gettime.c"
                "${mes-wip-2_13}/lib/linux/dup.c"
                "${mes-wip-2_13}/lib/linux/dup2.c"
                "${mes-wip-2_13}/lib/linux/execve.c"
                "${mes-wip-2_13}/lib/linux/fork.c"
                "${mes-wip-2_13}/lib/linux/fsync.c"
                "${mes-wip-2_13}/lib/linux/_getcwd.c"
                "${mes-wip-2_13}/lib/linux/gettimeofday.c"
                "${mes-wip-2_13}/lib/linux/ioctl3.c"
                "${mes-wip-2_13}/lib/linux/_open3.c"
                "${mes-wip-2_13}/lib/linux/_read.c"
                "${mes-wip-2_13}/lib/linux/time.c"
                "${mes-wip-2_13}/lib/linux/unlink.c"
                "${mes-wip-2_13}/lib/linux/waitpid.c"
                "${mes-wip-2_13}/lib/linux/x86-mes-mescc/syscall.c"
                "${mes-wip-2_13}/lib/linux/getpid.c"
                "${mes-wip-2_13}/lib/linux/kill.c"
              ];
            }
            + buildLibWithMes {
              name = "tcc";
              files = [
                "${mes-wip-2_13}/lib/ctype/islower.c"
                "${mes-wip-2_13}/lib/ctype/isupper.c"
                "${mes-wip-2_13}/lib/ctype/tolower.c"
                "${mes-wip-2_13}/lib/ctype/toupper.c"
                "${mes-wip-2_13}/lib/mes/abtod.c"
                "${mes-wip-2_13}/lib/mes/dtoab.c"
                "${mes-wip-2_13}/lib/mes/search-path.c"
                "${mes-wip-2_13}/lib/posix/execvp.c"
                "${mes-wip-2_13}/lib/stdio/fclose.c"
                "${mes-wip-2_13}/lib/stdio/fdopen.c"
                "${mes-wip-2_13}/lib/stdio/ferror.c"
                "${mes-wip-2_13}/lib/stdio/fflush.c"
                "${mes-wip-2_13}/lib/stdio/fopen.c"
                "${mes-wip-2_13}/lib/stdio/fprintf.c"
                "${mes-wip-2_13}/lib/stdio/fread.c"
                "${mes-wip-2_13}/lib/stdio/fseek.c"
                "${mes-wip-2_13}/lib/stdio/ftell.c"
                "${mes-wip-2_13}/lib/stdio/fwrite.c"
                "${mes-wip-2_13}/lib/stdio/printf.c"
                "${mes-wip-2_13}/lib/stdio/remove.c"
                "${mes-wip-2_13}/lib/stdio/snprintf.c"
                "${mes-wip-2_13}/lib/stdio/sprintf.c"
                "${mes-wip-2_13}/lib/stdio/sscanf.c"
                "${mes-wip-2_13}/lib/stdio/vfprintf.c"
                "${mes-wip-2_13}/lib/stdio/vprintf.c"
                "${mes-wip-2_13}/lib/stdio/vsnprintf.c"
                "${mes-wip-2_13}/lib/stdio/vsprintf.c"
                "${mes-wip-2_13}/lib/stdio/vsscanf.c"
                "${mes-wip-2_13}/lib/stdlib/calloc.c"
                "${mes-wip-2_13}/lib/stdlib/qsort.c"
                "${mes-wip-2_13}/lib/stdlib/strtod.c"
                "${mes-wip-2_13}/lib/stdlib/strtof.c"
                "${mes-wip-2_13}/lib/stdlib/strtol.c"
                "${mes-wip-2_13}/lib/stdlib/strtold.c"
                "${mes-wip-2_13}/lib/stdlib/strtoll.c"
                "${mes-wip-2_13}/lib/stdlib/strtoul.c"
                "${mes-wip-2_13}/lib/stdlib/strtoull.c"
                "${mes-wip-2_13}/lib/string/memmem.c"
                "${mes-wip-2_13}/lib/string/strcat.c"
                "${mes-wip-2_13}/lib/string/strchr.c"
                "${mes-wip-2_13}/lib/string/strlwr.c"
                "${mes-wip-2_13}/lib/string/strncpy.c"
                "${mes-wip-2_13}/lib/string/strrchr.c"
                "${mes-wip-2_13}/lib/string/strstr.c"
                "${mes-wip-2_13}/lib/string/strupr.c"
                "${mes-wip-2_13}/lib/stub/sigaction.c"
                "${mes-wip-2_13}/lib/stub/ldexp.c"
                "${mes-wip-2_13}/lib/stub/mprotect.c"
                "${mes-wip-2_13}/lib/stub/localtime.c"
                "${mes-wip-2_13}/lib/stub/sigemptyset.c"
                "${mes-wip-2_13}/lib/x86-mes-mescc/setjmp.c"
                "${mes-wip-2_13}/lib/linux/close.c"
                "${mes-wip-2_13}/lib/linux/rmdir.c"
                "${mes-wip-2_13}/lib/linux/stat.c"
              ];
            }
            +
            ''
              catm ''${out}/lib/x86-mes/libc+tcc.a ''${out}/lib/x86-mes/libc.a ''${out}/lib/x86-mes/tcc.a
              catm ''${out}/lib/x86-mes/libc+tcc.s ''${out}/lib/x86-mes/libc.s ''${out}/lib/x86-mes/tcc.s

              mkdir ''${out}/lib/linux
              mkdir ''${out}/lib/linux/x86-mes

              catm ''${out}/lib/linux/x86-mes/elf32-footer-single-main.hex2 \
                ${mes-wip-2_13}/lib/linux/x86-mes/elf32-footer-single-main.hex2
              catm ''${out}/lib/linux/x86-mes/elf32-header.hex2 \
                ${mes-wip-2_13}/lib/linux/x86-mes/elf32-header.hex2

              catm ''${out}/lib/x86-mes/x86.M1 ${mes-wip-2_13}/lib/x86-mes/x86.M1


              mkdir ''${out}/include
              mkdir ''${out}/include/linux
              mkdir ''${out}/include/mes
              mkdir ''${out}/include/sys

              catm ''${out}/include/mes/config.h
            ''
            +
            copyFiles {
              from = mes-wip-2_13;
              to = ''''${out}'';
              subPaths = [
                "include/alloca.h"
                "include/argz.h"
                "include/ar.h"
                "include/assert.h"
                "include/ctype.h"
                "include/dirent.h"
                "include/dirstream.h"
                "include/dlfcn.h"
                "include/endian.h"
                "include/errno.h"
                "include/fcntl.h"
                "include/features.h"
                "include/float.h"
                "include/getopt.h"
                "include/grp.h"
                "include/inttypes.h"
                "include/libgen.h"
                "include/limits.h"
                "include/locale.h"
                "include/math.h"
                "include/memory.h"
                "include/pwd.h"
                "include/setjmp.h"
                "include/signal.h"
                "include/stdarg.h"
                "include/stdbool.h"
                "include/stddef.h"
                "include/stdint.h"
                "include/stdio.h"
                "include/stdlib.h"
                "include/stdnoreturn.h"
                "include/string.h"
                "include/strings.h"
                "include/termio.h"
                "include/time.h"
                "include/unistd.h"
                "include/linux/syscall.h"
                "include/linux/x86/syscall.h"
                "include/mes/builtins.h"
                "include/mes/cc.h"
                "include/mes/constants.h"
                "include/mes/lib.h"
                "include/mes/lib-mini.h"
                "include/mes/mes.h"
                "include/mes/symbols.h"
                "include/sys/cdefs.h"
                "include/sys/dir.h"
                "include/sys/file.h"
                "include/sys/ioctl.h"
                "include/sys/mman.h"
                "include/sys/param.h"
                "include/sys/resource.h"
                "include/sys/select.h"
                "include/sys/stat.h"
                "include/sys/timeb.h"
                "include/sys/time.h"
                "include/sys/times.h"
                "include/sys/types.h"
                "include/sys/ucontext.h"
                "include/sys/user.h"
                "include/sys/wait.h"
              ];
            }
          );
    in
    derivation rec {
      name = "MesWip213BuildByM2";
      system = builtins.currentSystem;
      outputs = [ "out" ];
      srcs = buildScript;
      PATH = "${MesM2WithTools}/bin:${MesBootstrapWip213BuildByM2}/bin";
      GUILE_LOAD_PATH = "${nyacc}/module:${mes-wip-2_13}/mes/module:${mes-wip-2_13}/module";
      MES_ARENA = 20000000;
      MES_MAX_ARENA = 20000000;
      MES_STACK = 6000000;
      MES_DEBUG = 0;
      MES_PREFIX = "${mes-wip-2_13}";
      libdir = "${mes-wip-2_13}/lib";
      builder = "${MesM2WithTools}/bin/kaem";
      args = [ "--verbose" "--strict" "-f" "${srcs}" ];
    };
  toyboxTools =
    let
      buildScript = with sources; with generatedKaemScripts;
        builtins.toFile "toyboxTools.run"
          (
            ''
              mkdir ''${out}
              mkdir ''${out}/bin
            ''
          );
    in
    derivation rec {
      name = "toybox";
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
