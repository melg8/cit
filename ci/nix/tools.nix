# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

{ pkgs, pkgs2 }:
let
  pvs_studio_for_free = pkgs.callPackage ./pvs/how_to_use_pvs_studio_free.nix { };
  conform = pkgs.callPackage ./conform/default.nix { };
  git_lint = pkgs.callPackage ./git_lint/default.nix { };
  ls_lint = pkgs.callPackage ./ls_lint/default.nix { };
  remark_lint = (pkgs.callPackage ./remark/default.nix { }).shell.nodeDependencies;
  text_lint = (pkgs.callPackage ./text_lint/default.nix { }).shell.nodeDependencies;
  mega_linter = pkgs.callPackage ./mega_linter/default.nix { };
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
  pkgs.git # 397 MB
  pkgs.ccache # 33 MB
  pkgs.cmake # 96 MB
  pkgs.ninja
  pkgs.lcov
  pkgs.grcov

  # cpp analyzers and compilers.
  pkgs.cppcheck
  pkgs2.gcc12.cc # For gcov tool.
  pkgs2.gcc12 # Must be after gcc11.cc to provide right links in docker.
  pkgs.bintools-unwrapped # Linker: ar.
  pkgs.cpplint

  pkgs2.clang-tools_14
  clang_tidy_run
  pkgs2.clang_14 # Must be after gcc to provide right links in docker.
  conan_1_55

  pkgs2.perl # For building openssl for clang_14 version from sources.

  # go
  pkgs.git-sizer # 37 MB
  pkgs.gitleaks
  pkgs.shfmt
  pkgs.reviewdog
  conform # 57 MB
  ls_lint # 4.2 MB

  # gem
  # Together 316 MB  (295 on ruby 2.7)
  pkgs.mdl # ruby 2.7 261.4 MB
  git_lint # 294 MB

  # npm
  # Together 281 MB.
  pkgs.commitlint
  pkgs.nodePackages.cspell
  pkgs.nodePackages.prettier
  text_lint # 247 MB.
  remark_lint # 209 MB.

  # Haskell
  # Together 45 MB
  pkgs.shellcheck # 45 MB

  # python 3
  # Together 412 MB
  pkgs.python39Full
  pkgs.codespell # 140 MB
  pkgs.yamllint # 140 MB
  pkgs.cmake-format # 150 MB
  pkgs.gitlint
  pkgs.reuse
  mega_linter

  code_checker

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

  pkgs.just
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
