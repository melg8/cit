# Cit - Control it

[![Checks](https://github.com/melg8/cit/actions/workflows/checks.yml/badge.svg?branch=main)][1]
[![Build Status "main" branch](https://travis-ci.com/melg8/cit.svg?branch=main)](https://travis-ci.com/melg8/cit)
[![MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/melg8/cit/blob/main/license.md)
[![built with nix](https://img.shields.io/badge/Built_With-Nix-5277C3.svg?logo=nixos&labelColor=73C3D5)][2]

Control your c++ code using this continuous integration template.

**cit** - is a project template for development of C++ codebase.
It aggregates linters, static and dynamic analysis tools in single,
ready to use template.

## Main goals

- Explore capabilities of automatic tools and their impact on development

- Give unified access to linters and analysis tools

- Showcase techniques of testing different properties of codebase

- Provide reproducible builds and environments through
    [**docker**](https://www.docker.com/) and
    [**nix**](https://github.com/nix-community/awesome-nix)

- Encourage responsible attitude towards the development of quality software

## Current status

This project is under development, expect changes in api, setup and tools.
It uses opinionated set of rules and conventions for project,
fill free to change configuration files to best fit your personal needs.
You can fork and play with it, contributions to the source code,
issues and requests are welcome.

## How it works

1. [Nix](https://github.com/nixos/nix) used to reproducibly build
   tar archive with docker image containing all linters:

   ``` bash
   nix build -f ci/nix/docker_build.nix --enforce-determinism --repeat 1 --keep-failed
   ```

1. You can check that produced result has same hash sum as result
   produced by ci:

   ``` bash
   sha256sum result
   ```

1. Docker image than loaded from result:

   ``` bash
   docker load < result
   ```

1. To run all linters checks run inside that container use docker_run.sh script
   with all.sh script as argument:

   ``` bash
   ci/docker/docker_run.sh ci/checks/all.sh
   ```

1. To run more specific checks use docker_run.sh script with name of
   script from ci/checks:

   ``` bash
   ci/docker/docker_run.sh ci/checks/md_files_style.sh
   ```

1. To run interactive shell inside docker image:

   ``` bash
   ci/docker/run_shell.sh
   ```

1. If you don’t want to build docker image, you can load prepared docker
   image from docker hub:

   ``` bash
   ci/docker/docker_pull.sh
   ```

1. Deprecated: you can build old version of docker image using docker file:

   ``` bash
   ci/docker/build.sh
   ```

See more in [travis](.travis.yml) or
[GitHub workflows](.github/workflows/checks.yml) for basic how to use.

## Available tools

- **cmake**
  - [cmake-format](https://github.com/cheshirekow/cmake_format) GPLv3.0
  - [cmake-lint](https://github.com/cheshirekow/cmake_format) GPLv3.0

- **docker files**
  - [dockerfile_lint](https://github.com/projectatomic/dockerfile_lint) - MIT
  - [hadolint](https://github.com/hadolint/hadolint) - GPLv3.0

- **files and directories**
  - [ls-lint](https://github.com/loeffel-io/ls-lint) - MIT

- **git**
  - [git](https://github.com/git/git) - GPLv2,LGPLv2.1
  - [git-sizer](https://github.com/github/git-sizer) - MIT
  - [conform](https://github.com/talos-systems/conform) - MPL2.0
  - [commitlint](https://github.com/conventional-changelog/commitlint) - MIT
  - [gitlint](https://github.com/jorisroovers/gitlint) - MIT
  - [git-lint](https://github.com/bkuhlmann/git-lint) - Apache 2.0
  - [gitleaks](https://github.com/zricethezav/gitleaks) - MIT

- **md files**
  - [remark](https://github.com/remarkjs/remark) - MIT
  - [textlint](https://github.com/textlint/textlint) - MIT

- **shell**
  - [shellcheck](https://github.com/koalaman/shellcheck) - GPLv3.0

- **spelling**
  - [cspell](https://github.com/streetsidesoftware/cspell) - MIT
  - [codespell](https://github.com/codespell-project/codespell) - GPLv.2.0

- **yaml**
  - [yamllint](https://github.com/adrienverge/yamllint) - GPLv3.0

## Planned

- **cpp**
  - [cppcheck](https://github.com/danmar/cppcheck) - GPLv3.0
  - [clang-format](https://clang.llvm.org/docs/ClangFormat.html) - NCSA
  - [clang-tidy](https://clang.llvm.org/extra/clang-tidy) - NCSA
  - [jscpd](https://github.com/kucherenko/jscpd) - MIT

- **nix**
  - [nixpkgs-fmt](https://github.com/nix-community/nixpkgs-fmt) - Apache-2.0
  - [nix-linter](https://github.com/Synthetica9/nix-linter) - BSD 3-clause
  - [nix-shell][3] - LGPL-2.1 License

## Credits

Big thanks to contributors. You can see contributors in [credits](credits.md).

## License

**cit** template released under the terms of the MIT license.
See [license](license.md) for more information.

[1]: https://github.com/melg8/cit/actions/workflows/checks.yml
[2]: https://builtwithnix.org
[3]: https://nixos.org/manual/nix/unstable/command-ref/nix-shell.html
