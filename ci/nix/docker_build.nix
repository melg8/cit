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
    tag = "0.1.1";
    contents = [
                 pvs_studio_for_free
                 pkgs.codespell
                 pkgs.nodePackages_latest.textlint
                 pkgs.cmake-format
                 pkgs.yamllint
                 pkgs.git-sizer
                 pkgs.git
                 pkgs.mdl
                 pkgs.nodePackages.textlint
                 pkgs.shellcheck
                 pkgs.hadolint
                 ];
};
}