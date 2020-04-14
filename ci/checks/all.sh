#!/bin/sh

set -e

echo "yaml_files_style"
./ci/checks/yaml_files_style.sh

echo "md_files_style"
./ci/checks/md_files_style.sh

echo "spelling"
./ci/checks/spelling.sh

echo "shell_files_style"
./ci/checks/shell_files_style.sh

echo "git_symbols"
./ci/checks/git_symbols.sh

echo "conventional_git_style"
./ci/checks/conventional_git_style.sh

echo "directories_and_files_naming"
./ci/checks/directories_and_files_naming.sh

echo "docker_style"
./ci/checks/docker_style.sh
echo "All checks are passed."
