#!/bin/sh

set -e

DOCKER_PATH="./ci/docker/docker_files"
DOCKER_FILE=${DOCKER_PATH}/"Dockerfile"
VERSION="0.0.2"
TARGETS="hadolint_builder go_builder cit"
USER="melg8"

export DOCKER_CONTENT_TRUST=1

for TARGET in ${TARGETS}
do
    docker build \
            --file "${DOCKER_FILE}" \
            --target "${TARGET}" \
            --tag "${USER}"/"${TARGET}":"${VERSION}" \
            "${DOCKER_PATH}"
done