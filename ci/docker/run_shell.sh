#!/usr/bin/env sh

set -e
docker run --rm -it --ulimit nofile=1024 \
-v "$(pwd)":/home/user/work melg8/cit:0.0.6