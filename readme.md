<!--
SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>

SPDX-License-Identifier: MIT
-->

# Cit - Control it

[![Checks](https://github.com/melg8/cit/actions/workflows/checks.yml/badge.svg?branch=main)][1]
[![codecov](https://codecov.io/gh/melg8/cit/branch/develop/graph/badge.svg?token=S3LWXOZDEJ)][2]
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/melg8/cit.svg?logo=lgtm\&logoWidth=18)][5]
[![REUSE status](https://api.reuse.software/badge/github.com/melg8/cit)][10]
[![MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/melg8/cit/blob/main/license.md)
[![built with nix](https://img.shields.io/badge/Built_With-Nix-5277C3.svg?logo=nixos\&labelColor=73C3D5)][3]

Control your c++ code using this continuous integration template.

**cit** - is a project template for development of C++ codebase.
It aggregates linters, static and dynamic analysis tools in single,
ready to use template.

## Main goals of project

- Explore capabilities of automatic tools and their impact on development

- Give users of project unified access to linters and analysis tools

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

   ```bash
   nix build -f ci/nix/docker_build.nix --enforce-determinism --repeat 1 --keep-failed
   ```

1. You can check that produced result has same hash sum as result
   produced by ci:

   ```bash
   sha256sum result
   ```

1. Docker image than loaded from result:

   ```bash
   docker load < result
   ```

1. To run all linters checks run inside that container use docker_run.sh script
   with all.sh script as argument:

   ```bash
   ci/docker/docker_run.sh ci/checks/all.sh
   ```

1. To run more specific checks use docker_run.sh script with name of
   script from ci/checks:

   ```bash
   ci/docker/docker_run.sh ci/checks/md_files_style.sh
   ```

1. To run interactive shell inside docker image:

   ```bash
   ci/docker/run_shell.sh
   ```

1. If you don’t want to build docker image, you can load prepared docker
   image from docker hub:

   ```bash
   ci/docker/docker_pull.sh
   ```

See [GitHub workflows](.github/workflows/checks.yml) for usage details.

## Available tools

- **online services**
  - [GitHub actions][6] - check/build/run tests
  - [Codecov][7] - verify code coverage of tests
  - [lgtm](https://lgtm.com/) - build and analyze source code
  - [dependabot][8] - check dependencies

- **git**
  - [git](https://github.com/git/git) - GPLv2,LGPLv2.1
  - [git-sizer](https://github.com/github/git-sizer) - MIT
  - [conform](https://github.com/talos-systems/conform) - MPL2.0
  - [commitlint](https://github.com/conventional-changelog/commitlint) - MIT
  - [gitlint](https://github.com/jorisroovers/gitlint) - MIT
  - [git-lint](https://github.com/bkuhlmann/git-lint) - Apache 2.0
  - [gitleaks](https://github.com/zricethezav/gitleaks) - MIT

- **licenses**
  - [reuse](https://github.com/fsfe/reuse-tool) - Apache-2.0

- **files and directories**
  - [ls-lint](https://github.com/loeffel-io/ls-lint) - MIT

- **spelling**
  - [cspell](https://github.com/streetsidesoftware/cspell) - MIT
  - [codespell](https://github.com/codespell-project/codespell) - GPLv.2.0

- **cpp static analysis**
  - [clang](https://clang.llvm.org/) - warnings as errors checks - Apache 2.0
  - [clang-format](https://clang.llvm.org/docs/ClangFormat.html) - NCSA
  - [clang-tidy](https://clang.llvm.org/extra/clang-tidy) - NCSA
  - [cppcheck](https://github.com/danmar/cppcheck) - GPLv3.0
  - [cpplint](https://github.com/cpplint/cpplint) - BSD 3-clause
  - [gcc](https://gcc.gnu.org/) - warnings as errors checks - GPLv3.0

- **cpp dynamic analysis**
  - [clang](https://clang.llvm.org/) - sanitizers - Apache 2.0
  - [gcc](https://gcc.gnu.org/) - sanitizers - GPLv3.0
  - [grcov](https://github.com/mozilla/grcov) - code coverage - MPL-2.0

- **cpp examples of library usage**
  - [doctest](https://github.com/doctest/doctest) - MIT
  - [outcome](https://github.com/ned14/outcome) - Apache-2.0

- **cmake**
  - [cmake-format](https://github.com/cheshirekow/cmake_format) GPLv3.0
  - [cmake-lint](https://github.com/cheshirekow/cmake_format) GPLv3.0

- **shell**
  - [shellcheck](https://github.com/koalaman/shellcheck) - GPLv3.0

- **nix**
  - [nixpkgs-fmt](https://github.com/nix-community/nixpkgs-fmt) - Apache-2.0
  - [nix-linter](https://github.com/Synthetica9/nix-linter) - BSD 3-clause
  - [nix-shell][4] - LGPL-2.1 License

- **md files**
  - [remark](https://github.com/remarkjs/remark) - MIT
  - [textlint](https://github.com/textlint/textlint) - MIT

- **yaml**
  - [yamllint](https://github.com/adrienverge/yamllint) - GPLv3.0

## Planned

- **cpp tools**
  - [jscpd](https://github.com/kucherenko/jscpd) - MIT
  - [valgrind](https://valgrind.org/) - GPLv2.0
  - [libFuzzer](https://llvm.org/docs/LibFuzzer.html) - NCSA
  - [doxygen](https://github.com/doxygen/doxygen) - GPLv2.0
  - [standardese](https://github.com/standardese/standardese) - MIT
  - [mull](https://github.com/mull-project/mull) - Apache-2.0
  - [mutate_cpp](https://github.com/nlohmann/mutate_cpp) - MIT
  - [rapidcheck](https://github.com/emil-e/rapidcheck) - BSD-2-Clause
  - [clazy](https://github.com/KDE/clazy) - LGPLv2
  - [infer](https://github.com/facebook/infer) - MIT
  - [pvs studio](https://pvs-studio.com/en/pvs-studio/)

- **cpp compilers**
  - [msvc](https://docs.microsoft.com/en-us/cpp/?view=msvc-170) - Microsoft EULA
  - [MinGW](https://www.mingw-w64.org/) - GNU GPL

- **ci environments**
  - MacOS
  - Windows

- **examples of library usage**
  - [qt6](https://www.qt.io/product/qt6)
  - [openssl](https://github.com/openssl/openssl) - Apache-2.0
  - [gsl-lite](https://github.com/gsl-lite/gsl-lite) - MIT

- **gui testing**
  - [SikuliX1](https://github.com/RaiMan/SikuliX1) - MIT

- **reproducible builds testing**
  - [sha256sum](https://github.com/coreutils/coreutils) - GPL-3.0

- **unified console commands**
  - [just](https://github.com/casey/just) - CC0 1.0 Universal
  - [husky](https://github.com/typicode/husky) - MIT

- **approval testing**
  - [ApprovalTests.cpp][9] - Apache-2.0

- **microbenchmarking**

- **compile time benchmarking**

- **wiki about project**

- **logo and illustrations**

- **stable release versions and tags**

- **roadmap**

## Credits

Big thanks to contributors. You can see contributors in [credits](credits.md).

## License

**cit** template released under the terms of the MIT license.
See [license](license.md) for more information.

[1]: https://github.com/melg8/cit/actions/workflows/checks.yml

[2]: https://codecov.io/gh/melg8/cit

[3]: https://builtwithnix.org

[4]: https://nixos.org/manual/nix/unstable/command-ref/nix-shell.html

[5]: https://lgtm.com/projects/g/melg8/cit/context:cpp

[6]: https://github.com/melg8/cit/actions

[7]: https://app.codecov.io/gh/melg8/cit

[8]: https://github.com/dependabot/dependabot-core

[9]: https://github.com/approvals/ApprovalTests.cpp

[10]: https://api.reuse.software/info/github.com/melg8/cit
