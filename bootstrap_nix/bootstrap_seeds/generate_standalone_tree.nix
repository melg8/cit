let
  nixpkgs = import ../../ci/nix/pinned_nixpkgs.nix;
  pkgs = import nixpkgs { };
  sources = import ./sources.nix;
  generateKaemScripts = import ./generate_kaem_scripts.nix { inherit sources; };
  generate_kaem_scripts = import ./generate_kaem_scripts.nix {
    sources = builtins.listToAttrs (map (x: { name = x; value = x; })
      (builtins.attrNames (sources)));
  };
  base = import ./default.nix;
  generateEnv = import ./generate_env_from_json.nix;
  buildInputs = import ./build_inputs_1.nix;
in
with pkgs; rec {
  standalone-tree = stdenv.mkDerivation rec {
    pname = "kaem_build_tree";
    version = "0.1";

    dontUnpack = true;
    dontPatch = true;
    dontConfigure = true;
    dontBuild = true;
    dontFixup = true;
    depsBuildBuild = [ nukeReferences ];

    kaem_sources = lib.lists.zipListsWith (a: b: a + ":" + b)
      (builtins.attrNames (import ./sources.nix))
      (builtins.map (builtins.getAttr "outPath") (builtins.attrValues (import ./sources.nix)));

    installPhase = ''
      mkdir -p $out

      for source in $kaem_sources; do
        directory=''${source%:*}
        outPath=''${source#*:}
        echo "mkdir: $directory from sources out: $outPath"

        mkdir -p $directory
        cp -r $outPath/* $directory/
        nuke-refs $directory

        unset directory
        unset outPath
      done

      cp -r * $out/
      cp ${generate_kaem_scripts.kaem_run} $out/kaem.run
      sed -e 's,/nix/store/.*kaem1\.run,kaem1\.run,' -i $out/kaem.run
      cp ${generate_kaem_scripts.kaem_full_run} $out/kaem1.run
      sed -e 's,cd /build,out=\./kaem_full,' -i $out/kaem1.run

      nuke-refs $out
      nuke-refs $out/kaem.run
      nuke-refs $out/kaem1.run
    '';
  };

  build-standalone-tree = stdenv.mkDerivation {
    pname = "build-standalone-tree";
    version = "0.1";
    srcs = [ standalone-tree ];

    installPhase = ''
      mkdir build_tree
      cp -r ${standalone-tree}/* build_tree/
      cd build_tree
      ./bootstrap-seeds/POSIX/x86/kaem-optional-seed ./kaem.run
      mkdir -p $out/bin
      cp ./kaem_full/bin/* $out/bin/
    '';
    dontUnpack = true;
    dontPatch = true;
    dontConfigure = true;
    dontBuild = true;
    dontFixup = true;
  };
  # Needs "--option sandbox false" to work because it use information from
  # host nix-store.
  #  drv = base.kaem-env-test-2;
  drv = pkgs.hello;
  drvPath = builtins.unsafeDiscardStringContext drv.drvPath;
  pathSanitized = path: lib.replaceStrings [ "?" ] [ "-" ] path;
  jsonFor = drvPath: runCommand ''${pathSanitized drvPath}-json''
    { } ''
    export NIX_REMOTE=local?root=$PWD
    ${nix}/bin/nix show-derivation ${drvPath} --quiet > $out
  '';
  copyDirectBuildDependencies = drvPath: runCommand ''DirectDeps''
    { }
    ''
      export NIX_REMOTE=local?root=$PWD
      export USER=nobody
      echo path: ${drvPath}

      mkdir -p $out
      mkdir -p build/nix/store/
      cp ${drvPath} build/nix/store/derivation.dr
      ls -la build/nix/store

      export NIX_STATE_DIR=$TMPDIR
      ${nix}/bin/nix-store --init
      ${libfaketime}/bin/faketime -f "1970-01-01 00:00:01" \
      ${nix}/bin/nix-store --load-db < ${closureInfo { rootPaths = build/nix/store/derivation.dr; }}/registration


      ${nix}/bin/nix-store -qR build/nix/store/derivation.drv > $out/result
    '';

  escapeList = [ "'" "`" ">" "#" "\\n" ];
  buildCommandFor = genEnv: drvPath: runCommand ''${pathSanitized drvPath}-build.kaem''
    { } ''
    cat > $out << EOL
    ${genEnv (builtins.fromJSON (builtins.readFile (jsonFor drvPath)))}
    EOL
  '';
  script = generateEnv drv;
  generateEnvFromDrvPath = drvPath: generateEnv (builtins.readFile drvPath);
  testDependencyPath = "/nix/store/42d6drs0kdxl03l1can3p485iv4sbn4p-kaem_env_test_1_run.run";
  testDrvPath = "/nix/store/v4f07zfi7m48vh3wp6la32ypj9lz1729-mes-m2-with-tools.drv";
  testDrvImported = import testDrvPath;
  testFolderOutputs = pkgs.symlinkJoin {
    name = "myexample";
    runLocal = true;
    paths = [ testDrvImported ];
    postBuild = "echo link added";
  };


  testPopularity = referencesByPopularity base.kaem-env-test-2.drvPath;
  testPopularity1 = runCommand ''test-popularity-1''
    { } ''
    export NIX_REMOTE=local?root=$PWD
    cat ${testPopularity} | grep "[.]drv$" | xargs ${nix}/bin/nix show-derivation --quiet >> $out
  '';


  testFolder = copyAllRefs base.mes-m2-with-tools;



  drvPathWithoutContext = builtins.unsafeDiscardStringContext pkgs.hello.drvPath;

  drvFile = builtins.readFile drvPathWithoutContext;
  jsonForDrv = stdenv.mkDerivation {
    pname = "jsonForDrv";
    version = "0.1";
    src = ./.;
    buildInputs = [ git ];

    installPhase = ''
      export HOME=$TMP
      export NIX_STATE_DIR=$PWD
      export NIX_REMOTE=local?root=$PWD
      ${nix}/bin/nix-instantiate --readonly-mode --eval --strict -E "(import $src/default.nix).mes-m2-with-tools.drvPath"
      touch $out
    '';
    dontUnpack = true;
    dontPatch = true;
    dontConfigure = true;
    dontBuild = true;
    dontFixup = true;
  };

  directBuildDependencies =
    drv_path:
    let
      drv_file = builtins.readFile drv_path;
      storeDirRe = lib.replaceStrings [ "." ] [ "\\." ] builtins.storeDir;
      storeBaseRe = "[0-9a-df-np-sv-z]{32}-[+_?=a-zA-Z0-9-][+_?=.a-zA-Z0-9-]*";
      re = "(${storeDirRe}/${storeBaseRe})";
      inputs = lib.concatLists (lib.filter lib.isList (builtins.split re drv_file));
    in
    lib.unique inputs;

  testDirectDependencies = directBuildDependencies drvPath1;
  testDirectDependencies1 = copyDirectBuildDependencies drvPath1;

  useLocalKaem = scriptName: ''./bin_local_kaem --verbose --strict -f "${scriptName}"'';
  useAdvancedKaem = scriptName: ''${base.mes-m2-with-tools}/bin/new_kaem --verbose --strict -f "${scriptName}"'';

  copyAllRefs = drv:
    let
      drvPath = drv.drvPath;
      pathToIgnore = drv.outPath;
      usageOrder = [
        {
          drvPath = base.mes-m2-with-tools.drvPath;
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

        echo "${generateKaemScripts.build_kaem "local_kaem"}" > $out/init
        echo "${builtins.concatStringsSep "\n" generatedCommandFiles1.in_generatedCommands}" >> $out/init

        grep -rl "/nix/store/" $out | xargs sed -i "s/\/nix\/store/\.\/nix\/store/g"
      '';
  drvPath1 = builtins.unsafeDiscardStringContext base.kaem-env-test-2.drvPath;
  testDirectDependencies2 = copyAllRefs (import drvPath1);
}
