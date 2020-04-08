#!/bin/sh

set -e

export DOCKER_CONTENT_TRUST=1
DOCKER_FILE=./ci/docker/docker_files/Dockerfile

dockerfile_lint -f "${DOCKER_FILE}"
hadolint "${DOCKER_FILE}"
