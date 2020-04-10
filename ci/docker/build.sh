#!/bin/sh

set -e

docker build --tag melg8/denvy:0.0.1 ./ci/docker/docker_files/
