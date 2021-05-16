let
  sources = import ./sources.nix;
  generated_kaem_scripts = import ./generate_kaem_scripts.nix { inherit sources; };
in
with sources; rec  {
  mes-m2-with-tools = derivation rec {
    name = "mes-m2-with-tools";
    system = builtins.currentSystem;
    outputs = [ "out" ];
    srcs = generated_kaem_scripts.kaem_run;
    builder = "${bootstrap-seeds}/POSIX/x86/kaem-optional-seed";
    args = [ "${srcs}" ];
    allowedReferences = [];
    allowedRequisites = [];
  };
}
