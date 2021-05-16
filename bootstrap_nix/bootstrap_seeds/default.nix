let
  sources = import ./sources.nix;
  generated_kaem_scripts = import ./generate_kaem_scripts.nix { inherit sources; };
in
with sources; rec  {
  mescc-tools-mini = derivation rec {
    name = "mescc-tools-mini";
    system = builtins.currentSystem;
    outputs = [ "out" ];
    srcs = generated_kaem_scripts.kaem_run;
    builder = "${bootstrap-seeds}/POSIX/x86/kaem-optional-seed";
    args = [ "${srcs}" ];
    allowedReferences = [];
    allowedRequisites = [];
  };
}
