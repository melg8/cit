#!/bin/sh

set -e

COMMAND=$*
echo "command with args: " "${COMMAND}"

docker run --rm --ulimit nofile=1024 \
-v "$(pwd)":/home/user/work melg8/denvy:0.0.2 "${COMMAND}"
