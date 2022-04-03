#!/usr/bin/env sh

set -e

COMMAND=$*
echo "command with args: " "${COMMAND}"

docker container run --rm --ulimit nofile=1024 \
-v "$(pwd)":/home/user/work \
melg8/cit:0.0.7 nix-shell --run "${COMMAND}"
