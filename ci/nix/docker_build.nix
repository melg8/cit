let
  pkgs = import <nixpkgs> { };
  pvs_studio_for_free = pkgs.callPackage ./how_to_use_pvs_studio_free.nix { };
  conform = pkgs.callPackage ./conform/default.nix { };
  commit_lint = pkgs.callPackage ./commit_lint/default.nix { };
  cspell = (pkgs.callPackage ./cspell/default.nix { }).shell.nodeDependencies;
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
  text_lint = (pkgs.callPackage ./text_lint/default.nix { }).shell.nodeDependencies;
in
rec {
  world = pkgs.dockerTools.buildLayeredImage {
    name = "world";
    tag = "0.0.13";
    contents = [
      # All together 947 MB
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
      git_lint # 294 MB

      # npm
      # Together 271 MB.
      commit_lint # 224 MB
      cspell    # 209 MB.
      text_lint  # 247 MB.


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
    ];
  };
}
