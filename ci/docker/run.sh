#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

COMMAND=$*
echo "command with args: " "${COMMAND}"

docker container run --rm --ulimit nofile=1024 \
	-e REVIEWDOG_GITHUB_API_TOKEN \
	-e GITHUB_REPOSITORY \
	-e GITHUB_SHA \
	-e GITHUB_TOKEN \
	-e GITHUB_RUN_ID \
	-e PAT \
	-v "/var/run/docker.sock:/var/run/docker.sock:rw" \
	-v "$(pwd)":/home/user/work \
	melg8/cit:0.0.7 bash -c "${COMMAND}"
