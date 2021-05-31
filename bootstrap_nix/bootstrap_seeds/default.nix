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
      buildScript = with sources; with generatedKaemScripts;
        let
          src_path = "${live-bootstrap}/sysa/mescc-tools-extra/src";
        in
        builtins.toFile "MesCcToolsExtra1.run"
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
