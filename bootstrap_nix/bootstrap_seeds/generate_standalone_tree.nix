let
  nixpkgs = import ../../ci/nix/pinned_nixpkgs.nix;
  pkgs = import nixpkgs { };
  sources = import ./sources.nix;
  generateKaemScripts = import ./generate_kaem_scripts.nix { inherit sources; };
  base = import ./default.nix;
  generateEnv = import ./generate_env_from_json.nix;
  buildInputs = import ./build_inputs.nix;
in
with pkgs; rec {
  pathSanitized = path: lib.replaceStrings [ "?" ] [ "-" ] path;
  jsonFor = drvPath: runCommand ''${pathSanitized drvPath}-json''
    { } ''
    export NIX_REMOTE=local?root=$PWD
    ${nix}/bin/nix show-derivation ${drvPath} --quiet > $out
  '';
  buildCommandFor = genEnv: drvPath: runCommand ''${pathSanitized drvPath}-build.kaem''
    { } ''
    cat > $out << EOL
    ${genEnv (builtins.fromJSON (builtins.readFile (jsonFor drvPath)))}
    EOL
  '';
  useLocalKaem = scriptName: ''./bin_local_kaem --verbose --strict -f "${scriptName}"'';
  useAdvancedKaem = scriptName: ''${base.MesM2WithTools}/bin/kaem --verbose --strict -f "${scriptName}"'';

  copyAllRefs = initialRunner: initWith: drv:
    let
      drvPath = drv.drvPath;
      pathToIgnore = drv.outPath;
      usageOrder = [
        {
          drvPath = base.MesM2WithTools.drvPath;
          use = useLocalKaem;
          envGen = generateEnv.generateLocalKaemEnv;
        }
        {
          drvPath = drvPath;
          use = useAdvancedKaem;
          envGen = generateEnv.generateAdvancedKaemEnv;
        }
      ];
      handleUsageOrder =
        { in_inputs ? [ ]
        , in_generatedCommandFiles ? [ ]
        , in_generatedCommands ? [ ]
        }: { drvPath, use, envGen }:
        let
          inputs = buildInputs { drv_path = drvPath; visited = in_inputs; };
          generateEnvFor = buildCommandFor envGen;
          generatedCommandFiles = map generateEnvFor inputs;
          generatedCommands = map use generatedCommandFiles;
        in
        {
          in_inputs = in_inputs ++ inputs;
          in_generatedCommandFiles = in_generatedCommandFiles ++ generatedCommandFiles;
          in_generatedCommands = in_generatedCommands ++ generatedCommands;
        };
      generatedCommandFiles1 = builtins.foldl' handleUsageOrder { } usageOrder;
    in
    runCommand "copy-all-refs"
      rec  {
        exportReferencesGraph = [ "graph" drvPath ];
        ignoreOutputs = builtins.concatStringsSep " "
          (map (x: (import x).outPath)
            (buildInputs { drv_path = (import drvPath).drvPath; })) + " " +
        builtins.concatStringsSep " "
          (buildInputs { drv_path = drvPath; });
        allFiles = toString generatedCommandFiles1.in_generatedCommandFiles;
      }
      ''
        mkdir -p $out/nix/store
        while read path; do
          echo "path: $path"
          if echo $ignoreOutputs | grep -w -q $path; then
            echo "bad path found: $path"
          else
            cp -r $path $out/nix/store
          fi

          read dummy
          read nrRefs
          for ((i = 0; i < nrRefs; i++)); do read ref; done
        done < graph
        echo "NOW GRAPH 1"

        for file in $allFiles; do
            cp $file $out/nix/store
        done

        cp ${initialRunner} $out/init
        echo "${initWith}" > $out/kaem.run
        echo "${builtins.concatStringsSep "\n"
        generatedCommandFiles1.in_generatedCommands}" >> $out/kaem.run

        grep -rl "/nix/store/" $out | \
        xargs sed -i "s/\/nix\/store/\.\/nix\/store/g"
      '';
  drvPath1 = builtins.unsafeDiscardStringContext base.MesWip213BuildByM2.drvPath;
  prepareLocalKaem = generateKaemScripts.build_kaem "local_kaem";
  initialRunner = "${sources.bootstrap-seeds}/POSIX/x86/kaem-optional-seed";
  testDirectDependencies2 = copyAllRefs initialRunner prepareLocalKaem (import drvPath1);

  # Needs "--option sandbox false" to work because it use information from
  # host nix-store.
  build-standalone-tree = stdenv.mkDerivation {
    pname = "build-standalone-tree";
    version = "0.1";
    srcs = [ testDirectDependencies2 ];

    installPhase = ''
      mkdir build_tree
      cp -r ${testDirectDependencies2}/* build_tree/
      chmod -R 755 build_tree/*
      cd build_tree
      ./init
      mkdir -p $out
      cp -r ./ $out/
    '';
    dontUnpack = true;
    dontPatch = true;
    dontConfigure = true;
    dontBuild = true;
    dontFixup = true;
  };
}
