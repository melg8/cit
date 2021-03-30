let
  pkgs = import <nixpkgs> { };
  pvs_studio_for_free = pkgs.callPackage ./how_to_use_pvs_studio_free.nix { };
  conform = pkgs.callPackage ./conform/default.nix { };
  git_leaks = pkgs.callPackage ./git_leaks/default.nix { };
  git_lint = pkgs.callPackage ./git_lint/default.nix { };
  git_lint_py = pkgs.callPackage ./git_lint_py/default.nix { };
  ls_lint = pkgs.callPackage ./ls_lint/default.nix { };
  mdl = pkgs.mdl.override (old: {
    bundlerApp = attrs: old.bundlerApp (attrs // {
      ruby = pkgs.ruby_2_7;

    });
  }
  );
in
rec {
  world = pkgs.dockerTools.buildLayeredImage {
    name = "world";
    tag = "0.0.1";
    contents = [
      # All together 817 MB
      pvs_studio_for_free
      pkgs.git

      # go
      pkgs.git-sizer # 37 MB
      conform # 57 MB
      git_leaks # 44 MB
      ls_lint # 4.2 MB

      # gem
      # Together 316 MB  (295 on ruby 2.7)
      mdl # ruby 2.7 261.4 MB
      #pkgs.mdl # 260.4 MB
      git_lint # 294 MB

      # npm
      pkgs.nodePackages_latest.textlint # 259 MB

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
    ];
  };
}
