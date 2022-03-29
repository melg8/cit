let
  nixpkgs = import ./pinned_nixpkgs.nix;
  pkgs = import nixpkgs { };
  tools = import ./tools.nix { inherit pkgs; };
  inherit (pkgs.lib) concatStringsSep genList;
  nsswitch = ''
    passwd:    files systemd
    group:     files systemd
    shadow:    files

    hosts:     files mymachines dns myhostname
    networks:  files

    ethers:    files
    services:  files
    protocols: files
    rpc:       files
  '';
  passwd = ''
    root:x:0:0::/root:/sh
    user:x:1000:100::/home/user:/sh
    ${concatStringsSep "\n" (genList
     (i: "nixbld${toString (i + 1)}:x:${toString (i + 30001)}:30000::/var/empty:/run/current-system/sw/bin/nologin") 32)}
    nobody:x:65534:65534:Unprivileged account (don't use!):/var/empty:/run/current-system/sw/bin/nologin
  '';
  group = ''
    root:x:0:
    users:x:100:
    nixbld:x:30000:${concatStringsSep "," (genList (i: "nixbld${toString (i + 1)}") 32)}
    nogroup:x:65534:
  '';
  nix_conf = ''
    experimental-features = nix-command
  '';
in
rec {
  world = pkgs.dockerTools.buildImage rec {
    name = "melg8/cit";
    tag = "0.0.7";
    contents = with pkgs; [
      (writeTextDir "etc/nsswitch.conf" nsswitch)
      (writeTextDir "etc/passwd" passwd)
      (writeTextDir "etc/group" group)
      (writeTextDir "etc/nix/nix.conf" nix_conf)
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
    uid = user:1000;
    gid = users:100;

    extraCommands = import ./docker_extra_commands.nix {
      inherit nixpkgs;
      inherit pkgs;
      inherit contents;
    };
  };
}
