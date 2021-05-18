let
  sources = import ./sources.nix;
  generated_kaem_scripts = import ./generate_kaem_scripts.nix { inherit sources; };
  kaem_run = with sources; builtins.toFile "kaem.run" ''
    mkdir ''${out}


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
