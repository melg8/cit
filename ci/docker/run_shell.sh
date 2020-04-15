#!/bin/sh

set -e
docker run --rm -i -v "$(pwd)":/home/user melg8/yamllint:0.0.1 \
 "yamllint" "-s" "."