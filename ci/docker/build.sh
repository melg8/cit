#!/usr/bin/env bash

set -e


USER="melg8"
VERSION="0.0.3"
TARGETS=('hadolint_builder' 'go_builder' 'edge_builder' 'cit')
DOCKER_PATH="./ci/docker/docker_files"
DOCKER_FILE=${DOCKER_PATH}/"Dockerfile"

export DOCKER_BUILDKIT=1

# Change to 1 if you want to set up signing of your docker images.
export DOCKER_CONTENT_TRUST=0

for i in "${!TARGETS[@]}"; do
  TARGET="${TARGETS[i]}"

  TARGET_WITH_CACHE=(--target "${TARGET}")
    for ((j=0; j <= i; j++)); do
      TAGGED="${USER}"/"${TARGETS[j]}":"${VERSION}"
      TARGET_WITH_CACHE+=(--cache-from "${TAGGED}")
    done
   COMMAND=(build
            --pull \
            --build-arg "BUILDKIT_INLINE_CACHE=1" \
            --file "${DOCKER_FILE}" \
            "${TARGET_WITH_CACHE[@]}"
            --tag "${USER}"/"${TARGET}":"${VERSION}" \
            "${DOCKER_PATH}")
   docker "${COMMAND[@]}"
done
