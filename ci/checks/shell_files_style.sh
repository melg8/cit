#!/usr/bin/env sh

set -e

find . -name "*.sh" -exec shellcheck {} +
