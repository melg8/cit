let
  nixpkgs = (builtins.fetchGit {
    name = "nixos-unstable-2021-04-09";
    url = "https://github.com/nixos/nixpkgs";
    ref = "refs/heads/nixos-unstable";
    rev = "9e377a6ce42dccd9b624ae4ce8f978dc892ba0e2";
  });
  pkgs = import nixpkgs { overlays = [ (import ./overlay.nix) ]; };
  pvs_studio_for_free = pkgs.callPackage ./pvs/how_to_use_pvs_studio_free.nix { };
  conform = pkgs.callPackage ./conform/default.nix { };
  commit_lint = (pkgs.callPackage ./commit_lint/default.nix { }).shell.nodeDependencies;
  cspell = (pkgs.callPackage ./cspell/default.nix { }).shell.nodeDependencies;
  docker_file_lint = pkgs.callPackage ./docker_file_lint/docker_file_lint.nix { };
  git_leaks = pkgs.callPackage ./git_leaks/default.nix { };
  git_lint = pkgs.callPackage ./git_lint/default.nix { };
  git_lint_py = pkgs.callPackage ./git_lint_py/default.nix { };
  ls_lint = pkgs.callPackage ./ls_lint/default.nix { };
  mdl = pkgs.callPackage ./mdl/default.nix { };
  remark_lint = (pkgs.callPackage ./remark/default.nix { }).shell.nodeDependencies;
  text_lint = (pkgs.callPackage ./text_lint/default.nix { }).shell.nodeDependencies;

  nonRootShadowSetup = { user, uid, gid ? uid }: with pkgs; [
    (
      writeTextDir "etc/shadow" ''
        root:!x:::::::
        ${user}:!:::::::
      ''
    )
    (
      writeTextDir "etc/passwd" ''
        root:x:0:0::/root:${runtimeShell}
        ${user}:x:${toString uid}:${toString gid}::/home/${user}:
      ''
    )
    (
      writeTextDir "etc/group" ''
        root:x:0:
        ${user}:x:${toString gid}:
      ''
    )
    (
      writeTextDir "etc/gshadow" ''
        root:x::
        ${user}:x::
      ''
    )
  ];
in
rec {
  world = pkgs.dockerTools.buildImage rec {
    name = "melg8/cit";
    tag = "0.0.6";
    contents = [
      pkgs.bashInteractive
      pkgs.gnumake
      # All together 2.123 GB
      pvs_studio_for_free # 2.5 MB
      pkgs.git # 397 MB
      pkgs.ccache # 33 MB
      pkgs.cmake # 96 MB
      pkgs.clang_12 # 1.4 GB
      pkgs.gcc10 # 236 MB

      # go
      pkgs.git-sizer # 37 MB
      conform # 57 MB
      git_leaks # 44 MB
      ls_lint # 4.2 MB

      # gem
      # Together 316 MB  (295 on ruby 2.7)
      mdl # ruby 2.7 261.4 MB
      git_lint # 294 MB

      # npm
      # Together 281 MB.
      commit_lint # 224 MB
      cspell # 209 MB.
      text_lint # 247 MB.
      remark_lint # 209 MB.
      docker_file_lint # 209 MB.

      # Haskell
      # Together 52 MB
      pkgs.hadolint # 47 MB
      pkgs.shellcheck # 45 MB

      # python 3
      # Together 412 MB
      pkgs.codespell # 140 MB
      pkgs.yamllint # 140 MB
      pkgs.cmake-format # 150 MB
      git_lint_py # 139 MB

      # Potentially later.
      # Together 77 MB (+ 22 MB to total image size).
      pkgs.nix
      pkgs.nixpkgs-fmt
      pkgs.pkgsStatic.busybox
    ] ++ nonRootShadowSetup { uid = 1000; user = "user"; };
    config = {
      Cmd = [ "/bin/bash" ];
      User = "user";
      WorkingDir = "/home/user/work";
    };

    extraCommands = ''
      # This removes sharing of busybox and is not recommended. We do this
      # to make the example suitable as a test case with working binaries.
      #cp -r ${pkgs.pkgsStatic.busybox}/* .

      mkdir -p usr/bin
      ln -s /bin/env usr/bin/env

      for path in ${builtins.toString contents}; do
              echo "Linking path: $path"
              if [[ -e "$path/bin" ]] ; then
              for file in $(ls $path/bin/); do
                  if [[ ! -e bin/$file ]]; then
                    echo "Linking file: $path/bin/$file -> bin/$file"
                    if [[ -L bin/$file ]]; then
                        ln -sfn $path/bin/$file usr/bin/$file
                    else
                        ln -sfn $path/bin/$file bin/$file
                    fi

                  fi
              done
              fi
            done
      mkdir -p tmp
      chmod 1777 tmp

      if [[ -e lib ]] ; then
        cp -al lib usr/lib
      fi
    '';

  };
}
