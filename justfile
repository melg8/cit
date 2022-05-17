# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

# This list of available targets.
default:
  @just --list

# Choose what to do from the list.
choose:
  @just --choose

# Lint, build, and run all tests.
checkit:
  @just clean-all
  @just lint-all
  @just build-gcc
  @just build-clang

# Run all linters.
lint-all:
  @ci/checks/lint_all.sh

# Run concrete linter.
lint-with +linter_name:
  @ENABLE_LINTERS={{linter_name}} ci/checks/lint_all.sh

# Build with gcc, run tests.
build-gcc:
  @ci/builders/gcc/build.sh

# Build with clang, run tests.
build-clang:
  @ci/builders/clang/build.sh

# Build with gcc, run tests, create coverage report.
build-test-coverage:
  @ci/checks/coverage_report.sh

# Remove build and test folders.
clean-all:
  @rm -rf build_gcc
  @rm -rf build_clang
  @rm -rf report
  @echo "all cleaned"

# Build and load docker image.
docker-update-locally:
  @just docker-build
  @just docker-load

# Build, load and push docker image.
docker-update:
  @just docker-build
  @just docker-load
  @just docker-push

# Build docker image using nix.
docker-build:
  @ci/docker/build.sh

# Load resulting image of docker-build.
docker-load:
  @docker load < result

# Push loaded image to docker hub.
docker-push:
  @ci/docker/push.sh

# Pull image from docker hub.
docker-pull:
  @ci/docker/pull.sh

# Run commands inside docker image.
docker-run +commands:
  @ci/docker/run.sh {{commands}}

# Run command inside of nix shell in docker image.
docker-run-nix-shell +commands:
  @ci/docker/run_nix_shell.sh {{commands}}

# Run shell from docker.
docker-run-shell:
  @ci/docker/run_shell.sh
