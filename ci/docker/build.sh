#!/bin/sh

set -e

docker build --tag melg8/denvy:0.0.2 ./ci/docker/docker_files/
