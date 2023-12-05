#!/usr/bin/env bash

# SPDX-FileCopyrightText: © 2023 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

echo "ARGS: $*"

all_files=""
config_file=
config_arg="-c"

while [[ $# -gt 0 ]]; do
  case $1 in
  -c | --config)
    config_arg="$1"
    config_file="$2"
    shift 2
    ;;
  *)
    all_files+="$1 "
    shift
    ;;
  esac
done

echo "Config: $config_file"
echo "Files: $all_files"
echo "$all_files" | cspell stdin "$config_arg" "$config_file"
