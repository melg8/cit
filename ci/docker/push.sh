#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

VERSION="0.0.7"
TARGETS="cit"
USER="melg8"

export DOCKER_BUILDKIT=1
export DOCKER_CONTENT_TRUST=0

for TARGET in ${TARGETS}; do
	docker image push "${USER}"/"${TARGET}":"${VERSION}"
done
