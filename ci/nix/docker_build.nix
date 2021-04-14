let
  nixpkgs = (builtins.fetchGit {
    name = "nixos-unstable-2021-04-09";
    url = "https://github.com/nixos/nixpkgs";
    ref = "refs/heads/nixos-unstable";
    rev = "9e377a6ce42dccd9b624ae4ce8f978dc892ba0e2";
  });
  pkgs = import nixpkgs { };
  tools = import ./tools.nix { pkgs = pkgs; };
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
    contents = tools
      ++ nonRootShadowSetup { uid = 1000; user = "user"; };
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
