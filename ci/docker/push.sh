#!/bin/sh

set -e

VERSION="0.0.2"
TARGETS="hadolint_builder go_builder cit"
USER="melg8"

for TARGET in ${TARGETS}
do
    docker push "${USER}"/"${TARGET}":"${VERSION}"
done