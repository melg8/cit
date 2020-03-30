#!/bin/sh

set -e
conform enforce
commitlint -t HEAD
gitlint --commits HEAD