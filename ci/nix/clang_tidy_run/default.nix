# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

{ stdenv, fetchurl }:
let
  run_clang_tidy_py =
    fetchurl {
      url = "https://raw.githubusercontent.com/llvm/llvm-project/main/clang-tools-extra/clang-tidy/tool/run-clang-tidy.py";
      sha256 = "sha256-DIz5oT/Kl9wgryxu9z4LsGLXoTZdyODrhvUtz5Z8lUw=";
    };
in
stdenv.mkDerivation {
  name = "clang_tidy_run";
  phases = "installPhase";
  installPhase = ''
    mkdir -p $out/bin
    cp ${run_clang_tidy_py} $out/bin/clang-tidy-run
    chmod +x $out/bin/clang-tidy-run
  '';
}
