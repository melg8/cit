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
  kaemEnvTest = derivation rec {
    name = "kaemEnvTest";
    system = builtins.currentSystem;
    outputs = [ "out" ];
    srcs = kaemRun;
    PATH = "${MesM2WithTools}:${MesM2WithTools}/bin";
    builder = "${MesM2WithTools}/bin/new_kaem";
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
