#!/bin/sh

set -e
docker run --rm -it --ulimit nofile=1024 \
-v "$(pwd)":/home/user melg8/denvy:0.0.2