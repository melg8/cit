let
  nixpkgs = import ./pinned_nixpkgs.nix;
  pkgs = import nixpkgs { };
  tools = import ./tools.nix { pkgs = pkgs; };
  nonRootShadowSetup = import ./docker_shadow_setup.nix { pkgs = pkgs; };
  inherit (pkgs.lib) concatStringsSep genList;
  passwd = ''
    root:x:0:0::/root:/sh
    user:x:1000:100::/home/user:/sh
    ${concatStringsSep "\n" (genList
     (i: "nixbld${toString (i + 1)}:x:${toString (i + 30001)}:30000::/var/empty:/run/current-system/sw/bin/nologin") 32)}
  '';

  group = ''
    root:x:0:
    users:x:100:
    nixbld:x:30000:${concatStringsSep "," (genList (i: "nixbld${toString (i + 1)}") 32)}
    nogroup:x:65534:
  '';
in
rec {
  world = pkgs.dockerTools.buildImage rec {
    name = "melg8/cit";
    tag = "0.0.6";
    contents = with pkgs; [
      (writeTextDir "etc/passwd" passwd)
      (writeTextDir "etc/group" group)
    ] ++ tools;
    config = {
      Cmd = [ "sh" ];
      User = "user";
      WorkingDir = "/home/user/work";
      Env = [
        "NODE_PATH=/usr/lib/node_modules"
        "NIX_PAGER=cat"
        "USER=user"
        "NIX_PATH=nixpkgs=${nixpkgs}"
      ];
    };
    uid = 1000;
    gid = 100;

    extraCommands = import ./docker_extra_commands.nix {
      inherit nixpkgs;
      inherit pkgs;
      inherit contents;
    };
  };
}
