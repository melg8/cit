# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

{ writeShellScriptBin }:
writeShellScriptBin "git-spell" ''
  set -e
  git log --pretty="%B" | cspell stdin "$@"
''
