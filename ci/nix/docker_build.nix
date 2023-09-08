# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

let
  nixpkgs = import ./pinned_nixpkgs.nix;
  nixpkgs2 = import ./pinned_nixpkgs_2.nix;
  nixpkgs3 = import ./pinned_nixpkgs_3.nix;
  pkgs = import nixpkgs { };
  pkgs2 = import nixpkgs2 { };
  pkgs3 = import nixpkgs3 { };
  tools = import ./tools.nix {
    inherit pkgs; inherit pkgs2; inherit pkgs3;
  };
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
  hasIncludeDir = path: pkgs.lib.pathExists (toString path + "/include");
  contentsWithInclude = pkgs.lib.filter hasIncludeDir;
  toStringWithInclude = single_path: toString single_path + "/include";
  includeStrings = allContents: pkgs.lib.concatStringsSep " -isystem "
    (map toStringWithInclude (contentsWithInclude allContents));
  hasBinDir = path: pkgs.lib.pathExists (toString path + "/bin");
  contentsWithBin = pkgs.lib.filter hasBinDir;
  toStringWithBin = single_path: toString single_path + "/bin";
  binStrings = allContents: pkgs.lib.concatStringsSep ":"
    (map toStringWithBin (contentsWithBin allContents));

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
      Cmd = [ "bash" ];
      User = "user";
      WorkingDir = "/home/user/work";
      Env = [
        "NODE_PATH=/usr/lib/node_modules"
        "NIX_PAGER=cat"
        "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:${binStrings contents}"
        "USER=user"
        "NIX_PATH=nixpkgs=${nixpkgs}"
        "NIX_CFLAGS_COMPILE=-isystem ${includeStrings contents}"
        "NIX_CC_WRAPPER_TARGET_HOST_x86_64_unknown_linux_gnu=1"
        "LANG=C.UTF-8"
        "MEGALINTER_FLAVOR=cit"
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
