let
    pkgs = import <nixpkgs> {};
    pvs_studio_for_free = pkgs.callPackage ./how_to_use_pvs_studio_free.nix { };
in rec {
 hello = pkgs.dockerTools.buildLayeredImage {
    name = "hello2";
    tag = "0.0.2";
    config.Cmd = [ "${pkgs.hello}" ];
};
 world = pkgs.dockerTools.buildLayeredImage {
    name = "world";
    tag = "0.0.10";
    contents = [ pvs_studio_for_free ];
    extraCommands = ''
          # This removes sharing of busybox and is not recommended. We do this
          # to make the example suitable as a test case with working binaries.
          cp -r ${pkgs.pkgsStatic.busybox}/* .
        '';
};
}