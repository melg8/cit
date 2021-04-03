#!/usr/bin/env sh

set -e

COMMAND=$*
echo "command with args: " "${COMMAND}"

docker run --rm --ulimit nofile=1024 \
-v "$(pwd)":/home/user/work melg8/cit:0.0.6 "${COMMAND}"
