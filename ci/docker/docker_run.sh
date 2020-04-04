#!/bin/sh

set -e

COMMAND=$*
echo "command with args: " "${COMMAND}"

docker run --rm -v "$(pwd)":/home/user melg8/denvy /bin/bash -c "${COMMAND}"
