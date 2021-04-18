let
  nixpkgs = import ./pinned_nixpkgs.nix;
  pkgs = import nixpkgs { };
  tools = import ./tools.nix { pkgs = pkgs; };
  nonRootShadowSetup = import ./docker_shadow_setup.nix { pkgs = pkgs; };
  inherit (pkgs.lib) concatStringsSep genList;
  passwd = ''
    root:x:0:0::/root:/sh
    ${concatStringsSep "\n" (genList
     (i: "nixbld${toString (i + 1)}:x:${toString (i + 30001)}:30000::/var/empty:/run/current-system/sw/bin/nologin") 32)}
  '';

  group = ''
    root:x:0:
    nixbld:x:30000:${concatStringsSep "," (genList (i: "nixbld${toString (i + 1)}") 32)}
    nogroup:x:65534:
  '';
in
rec {
  world = pkgs.dockerTools.buildImageWithNixDb rec {
    name = "melg8/cit";
    tag = "0.0.6";
    contents = with pkgs; [
      (writeTextDir "etc/passwd" passwd)
      (writeTextDir "etc/group" group)
      nixpkgs
    ] ++ tools;
    config = {
      Cmd = [ "sh" ];
      User = "root";
      WorkingDir = "/tmp/work";
      Env = [
        "NODE_PATH=/usr/lib/node_modules"
        "NIX_PAGER=cat"
        "USER=root"
        "NIX_PATH=nixpkgs=${nixpkgs}"
      ];
    };
    extraCommands = import ./docker_extra_commands.nix {
      inherit contents;
    };
  };
}
