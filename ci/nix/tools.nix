{ pkgs }:
let
  pvs_studio_for_free = pkgs.callPackage ./pvs/how_to_use_pvs_studio_free.nix { };
  conform = pkgs.callPackage ./conform/default.nix { };
  commit_lint = (pkgs.callPackage ./commit_lint/default.nix { }).shell.nodeDependencies;
  cspell = (pkgs.callPackage ./cspell/default.nix { }).shell.nodeDependencies;
  docker_file_lint = pkgs.callPackage ./docker_file_lint/docker_file_lint.nix { };
  git_leaks = pkgs.callPackage ./git_leaks/default.nix { };
  git_lint = pkgs.callPackage ./git_lint/default.nix { };
  git_lint_py = pkgs.callPackage ./git_lint_py/default.nix { };
  ls_lint = pkgs.callPackage ./ls_lint/default.nix { };
  mdl = pkgs.callPackage ./mdl/default.nix { };
  remark_lint = (pkgs.callPackage ./remark/default.nix { }).shell.nodeDependencies;
  text_lint = (pkgs.callPackage ./text_lint/default.nix { }).shell.nodeDependencies;
in
[
  pkgs.cacert
  pkgs.binutils



  # All together 2.123 GB
  pvs_studio_for_free # 2.5 MB
  pkgs.git # 397 MB
  pkgs.ccache # 33 MB
  pkgs.cmake # 96 MB
  pkgs.clang_12 # 1.4 GB


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
  # Together 281 MB.
  commit_lint # 224 MB
  cspell # 209 MB.
  text_lint # 247 MB.
  remark_lint # 209 MB.
  docker_file_lint # 209 MB.

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
  pkgs.pkgsStatic.busybox

  ### For debug only.
  pkgs.nix-tree

  ### Add deps for docker to be hermetic.
  pkgs.gcc10 # 236 MB

  pkgs.gccStdenv
  pkgs.bashInteractive
  pkgs.bashInteractive.dev
  pkgs.bashInteractive.doc
  pkgs.bashInteractive.info
  pkgs.bashInteractive.man

  pkgs.gawk
  pkgs.gnumake
  pkgs.ed
  pkgs.gnupatch
  pkgs.patchelf
  pkgs.findutils
  pkgs.diffutils
]
