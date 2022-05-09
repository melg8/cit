# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

{ writeShellScriptBin }:
writeShellScriptBin "bash-exec" ''
  if [[ -x "$1" ]]; then
    exit 0;
  else
    echo "Error: File:[$1] is not executable";
  exit 1;
  fi
''
