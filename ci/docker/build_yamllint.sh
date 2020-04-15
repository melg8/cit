#!/bin/sh

set -e

docker build --tag melg8/yamllint:0.0.1 ./ci/docker/docker_files/yamllint
