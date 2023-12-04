#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

COMMAND=$*
echo "command with args: " "${COMMAND}"

# --privileged flag used for docker incompatibility with glibc-2.34 workaround.

docker container run --rm --ulimit nofile=2048 \
	-e REVIEWDOG_GITHUB_API_TOKEN \
	-e GITHUB_REPOSITORY \
	-e GITHUB_SHA \
	-e GITHUB_TOKEN \
	-e GITHUB_RUN_ID \
	-e GITHUB_ACTIONS \
	-e GITHUB_EVENT_PATH="/home/user/event.json" \
	-e PAT \
	--privileged \
	-v "/var/run/docker.sock:/var/run/docker.sock:rw" \
	-v "${GITHUB_EVENT_PATH}":"/home/user/event.json" \
	-v "$(pwd)":/home/user/work \
	melg8/cit:0.0.7 nix-shell --run "${COMMAND}"
