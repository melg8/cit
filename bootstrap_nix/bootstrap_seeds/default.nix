let
  sources = import ./sources.nix;
  generated_kaem_scripts = import ./generate_kaem_scripts.nix { inherit sources; };
  kaem_run = with sources; builtins.toFile "kaem.run" ''
    mkdir ''${out}
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
  '';
in
with sources;
rec  {
  mes-m2-with-tools = derivation rec {
    name = "mes-m2-with-tools";
    system = builtins.currentSystem;
    outputs = [ "out" ];
    srcs = generated_kaem_scripts.kaem_run;
    builder = "${bootstrap-seeds}/POSIX/x86/kaem-optional-seed";
    args = [ "${srcs}" ];
    allowedReferences = [ ];
    allowedRequisites = [ ];
  };
  kaem-env-test = derivation rec {
    name = "kaem-env-test";
    system = builtins.currentSystem;
    outputs = [ "out" ];
    srcs = kaem_run;
    PATH = "${mes-m2-with-tools}:${mes-m2-with-tools}/bin";
    builder = "${mes-m2-with-tools}/bin/new_kaem";
    args = [ "--verbose" "--strict" "-f" "${srcs}" ];
    allowedReferences = [ ];
    allowedRequisites = [ ];
  };
}
