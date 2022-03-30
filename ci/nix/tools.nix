{ pkgs }:
let
  pvs_studio_for_free = pkgs.callPackage ./pvs/how_to_use_pvs_studio_free.nix { };
  gsl_lite = pkgs.callPackage ./gsl_lite/default.nix { };
  conform = pkgs.callPackage ./conform/default.nix { };
  git_lint = pkgs.callPackage ./git_lint/default.nix { };
  ls_lint = pkgs.callPackage ./ls_lint/default.nix { };
  remark_lint = (pkgs.callPackage ./remark/default.nix { }).shell.nodeDependencies;
  text_lint = (pkgs.callPackage ./text_lint/default.nix { }).shell.nodeDependencies;
in
[
  pvs_studio_for_free # 2.5 MB
  pkgs.git # 397 MB
  pkgs.ccache # 33 MB
  pkgs.cmake # 96 MB
  pkgs.ninja

  # cpp analyzers and compilers
  pkgs.cppcheck
  pkgs.llvmPackages_13.libclang # clang + clang-tidy + clang-format etc.

  pkgs.clang_13
  pkgs.gcc11
  pkgs.bintools-unwrapped # Linker: ar.
  pkgs.doctest
  gsl_lite

  # go
  pkgs.git-sizer # 37 MB
  conform # 57 MB
  pkgs.gitleaks
  ls_lint # 4.2 MB

  # gem
  # Together 316 MB  (295 on ruby 2.7)
  pkgs.mdl # ruby 2.7 261.4 MB
  git_lint # 294 MB

  # npm
  # Together 281 MB.
  pkgs.commitlint
  pkgs.nodePackages.cspell
  text_lint # 247 MB.
  remark_lint # 209 MB.

  # Haskell
  # Together 45 MB
  pkgs.shellcheck # 45 MB

  # python 3
  # Together 412 MB
  pkgs.codespell # 140 MB
  pkgs.yamllint # 140 MB
  pkgs.cmake-format # 150 MB
  pkgs.gitlint

  # Potentially later.
  # Together 77 MB (+ 22 MB to total image size).
  pkgs.nix
  pkgs.nixpkgs-fmt
  pkgs.nix-linter
  pkgs.pkgsStatic.busybox

  ### For debug only.
  pkgs.nix-tree

  ### Add deps for docker to be hermetic.
  pkgs.cacert
  pkgs.binutils

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
  pkgs.stdenv
]
