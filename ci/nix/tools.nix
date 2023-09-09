# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

{ pkgs, pkgs2, pkgs3 }:
let
  pvs_studio_for_free = pkgs.callPackage ./pvs/how_to_use_pvs_studio_free.nix { };
  conform = pkgs.callPackage ./conform/default.nix { };
  git_lint = pkgs.callPackage ./git_lint/default.nix { };
  ls_lint = pkgs.callPackage ./ls_lint/default.nix { };
  remark_lint = (pkgs.callPackage ./remark/default.nix { }).shell.nodeDependencies;
  text_lint = (pkgs.callPackage ./text_lint/default.nix { }).shell.nodeDependencies;
  mega_linter = pkgs3.callPackage ./mega_linter/default.nix { };
  bash_exec = pkgs.callPackage ./bash_exec/default.nix { };
  git_spell = pkgs.callPackage ./git_spell/default.nix { };
  ls_spell = pkgs.callPackage ./ls_spell/default.nix { };
  nixpkgs_fmt_wrapper = pkgs.callPackage ./nixpkgs_fmt_wrapper/default.nix { };
  clang_tidy_run = pkgs.callPackage ./clang_tidy_run/default.nix { };
  code_checker = pkgs.callPackage ./code_checker/default.nix { };
  conan_1_55 = pkgs2.conan.overrideAttrs (_finalAttrs: _previousAttrs: rec {
    version = "1.55.0";
    src = pkgs2.fetchFromGitHub {
      owner = "conan-io";
      repo = "conan";
      rev = version;
      hash = "sha256-YIw+D6C2BgOb7pkjViOIPTidaoLr1MWntY3nbDyjShc=";
    };
  });
in
[
  # Scripts.
  bash_exec
  git_spell
  ls_spell
  nixpkgs_fmt_wrapper

  pvs_studio_for_free # 2.5 MB
  pkgs3.git
  pkgs3.ccache
  pkgs3.cmake
  pkgs3.ninja
  pkgs3.lcov
  pkgs3.grcov

  # cpp analyzers and compilers.
  pkgs3.cppcheck
  pkgs3.gcc13.cc # For gcov tool.
  pkgs3.gcc13 # Must be after gcc13.cc to provide right links in docker.
  pkgs3.bintools-unwrapped # Linker: ar.
  pkgs3.cpplint

  pkgs3.clang-tools_15
  # clang_tidy_run
  pkgs3.clang_15 # Must be after gcc to provide right links in docker.
  pkgs3.conan

  pkgs3.perl # For building openssl for clang_14 version from sources.

  # go
  pkgs2.git-sizer # 37 MB
  pkgs2.gitleaks
  pkgs2.shfmt
  pkgs2.reviewdog
  conform # 57 MB
  ls_lint # 4.2 MB

  # gem
  # Together 316 MB  (295 on ruby 2.7)
  pkgs.mdl # ruby 2.7 261.4 MB
  #git_lint # 294 MB

  # npm
  # Together 281 MB.
  pkgs2.commitlint
  pkgs.nodePackages.cspell
  pkgs.nodePackages.prettier
  text_lint # 247 MB.
  remark_lint # 209 MB.

  # Haskell
  # Together 45 MB
  pkgs2.shellcheck # 45 MB

  # python 3
  # Together 412 MB
  pkgs.python39Full
  pkgs2.codespell # 140 MB
  pkgs2.yamllint # 140 MB
  pkgs2.cmake-format # 150 MB
  pkgs2.gitlint
  pkgs2.reuse
  # mega_linter

  code_checker

  # Potentially later.
  # Together 77 MB (+ 22 MB to total image size).
  pkgs3.nix
  pkgs3.nixpkgs-fmt
  pkgs2.nix-linter
  pkgs3.pkgsStatic.busybox

  ### For debug only.
  pkgs3.nix-tree

  ### Add deps for docker to be hermetic.
  pkgs3.cacert
  pkgs3.binutils

  pkgs3.just
  pkgs3.bashInteractive
  pkgs3.bashInteractive.dev
  pkgs3.bashInteractive.doc
  pkgs3.bashInteractive.info
  pkgs3.bashInteractive.man

  pkgs3.gawk
  pkgs3.gnumake
  pkgs3.ed
  pkgs3.gnupatch
  pkgs3.patchelf
  pkgs3.findutils
  pkgs3.diffutils
]
