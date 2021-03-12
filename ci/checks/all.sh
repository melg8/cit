#!/usr/bin/env sh

set -e

command time -v ./ci/checks/yaml_files_style.sh
command time -v ./ci/checks/md_files_style.sh
command time -v ./ci/checks/spelling.sh
command time -v ./ci/checks/shell_files_style.sh
command time -v ./ci/checks/git_symbols.sh
command time -v ./ci/checks/conventional_git_style.sh
command time -v ./ci/checks/directories_and_files_naming.sh
command time -v ./ci/checks/docker_style.sh
command time -v ./ci/checks/cmake_style.sh
echo "All checks are passed."
