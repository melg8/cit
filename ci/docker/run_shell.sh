#!/usr/bin/env sh

set -e
docker container run --rm -it --ulimit nofile=1024 \
-v "$(pwd)":/home/user/work --network none  melg8/cit:0.0.6
