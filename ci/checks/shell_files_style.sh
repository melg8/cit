#!/bin/sh

set -e

find . -name "*.sh" -exec shellcheck {} +
