let
  nixpkgs = (builtins.fetchGit {
    name = "nixos-unstable-2021-04-09";
    url = "https://github.com/nixos/nixpkgs";
    ref = "refs/heads/nixos-unstable";
    rev = "9e377a6ce42dccd9b624ae4ce8f978dc892ba0e2";
  });
  pkgs = import nixpkgs { };
  tools = import ./tools.nix { pkgs = pkgs; };
  nonRootShadowSetup = import ./docker_shadow_setup.nix { pkgs = pkgs; };
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
      Env = [ "NODE_PATH=/usr/lib/node_modules" ];
    };
    extraCommands = import ./docker_extra_commands.nix { inherit contents; };
  };
}
