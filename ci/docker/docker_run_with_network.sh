#!/usr/bin/env sh

# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

set -e

COMMAND=$*
echo "command with args: " "${COMMAND}"

docker container run --rm --ulimit nofile=1024 \
	-v "$(pwd)":/home/user/work \
	melg8/cit:0.0.7 "${COMMAND}"
