# SPDX-FileCopyrightText: © 2023 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

FROM ubuntu:23.04 AS cit

LABEL name=cit version=0.1.0

ARG DEBIAN_FRONTEND=noninteractive
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

RUN apt-get -y update \
 && apt-get -y install --no-install-recommends \
    software-properties-common=0.99.35 \
    gpg \
    gpg-agent \
 && add-apt-repository ppa:apt-fast/stable \
 && apt-get -y install --no-install-recommends \
    apt-fast=1.9.* \
 && apt-fast update \
 && apt-fast -y install --no-install-recommends \
    build-essential=12.9* \
    gcc-13=13.* \
    g++-13=13.* \
    clang-16=1:16* \
    libclang-16-dev=1:16.* \
    libclang-rt-16-dev=1:16.* \
    llvm-16-dev=1:16.* \
    git=1:2.39.* \
    cmake=3.25.* \
    ninja-build=1.11.* \
    grcov=0.8.* \
 && update-alternatives --install \
    /usr/bin/gcc gcc /usr/bin/gcc-13 60 --slave \
    /usr/bin/g++ g++ /usr/bin/g++-13 \
 && update-alternatives --install \
    /usr/bin/clang clang /usr/bin/clang-16 60 --slave \
    /usr/bin/clang++ clang++ /usr/bin/clang++-16

RUN apt-fast -y install --no-install-recommends \
    python3-full \
    python3-pip \
 && rm /usr/lib/python3.11/EXTERNALLY-MANAGED \
 && pip install conan \
 && pip install -U setuptools

RUN apt-fast -y install --no-install-recommends \
    shellcheck=0.9.* \
    shfmt=3.6.*

RUN apt-fast -y install --no-install-recommends \
    cmake-format=0.6.*

RUN apt-fast -y install --no-install-recommends \
    git-sizer=1.5.*

RUN apt-fast -y install --no-install-recommends \
    golang-go=2:1.* \
 && go install github.com/siderolabs/conform/cmd/conform@latest \
 && mv ~/go/bin/* /usr/local/bin/

RUN apt-fast update \
 && apt-fast -y install --no-install-recommends \
    gitlint \
 && apt-fast -y autoremove \
 && apt-fast clean

RUN apt-fast update \
 && apt-fast -y install --no-install-recommends \
    nodejs=18.13.* \
    npm=9.2.* \
 && apt-fast -y autoremove \
 && apt-fast clean

RUN npm i -g @commitlint/cli \
  -g cspell \
  -g @cspell/dict-ru_ru

RUN pip install whitespace-format

COPY ./megalinter/patches/megalinter.patch /tmp/megalinter.patch

RUN mkdir /tmp/build \
 && cd /tmp/build \
 && git clone --branch v7.6.0 --depth 1 \
    https://github.com/oxsecurity/megalinter.git \
 && cd megalinter \
 && mv /tmp/megalinter.patch ./megalinter.patch \
 && git apply ./megalinter.patch \
 && PYTHONDONTWRITEBYTECODE=1 python3 ./megalinter/setup.py install \
 && mkdir /megalinter-descriptors \
 && cp -r ./megalinter/descriptors/* /megalinter-descriptors

COPY ./megalinter.sh /usr/bin/megalinter
RUN chmod +x /usr/bin/megalinter
COPY ./megalinter/descriptors /megalinter-descriptors

RUN npm i -g @ls-lint/ls-lint

COPY ./linters/bash_exec/bash_exec.sh /usr/bin/bash-exec
COPY ./linters/git_spell/git_spell.sh /usr/bin/git-spell
COPY ./linters/ls_spell/ls_spell.sh /usr/bin/ls-spell

RUN pip install codespell

RUN apt-fast update \
 && apt-fast -y install --no-install-recommends \
    ruby3.1 \
 && gem install mdl -v 0.9.0


RUN npm i -g --no-cache --ignore-scripts \
    remark-cli

RUN npm i -g --no-cache --ignore-scripts \
  textlint \
  textlint-rule-no-dead-link \
  textlint-rule-no-start-duplicated-conjunction \
  textlint-rule-max-comma \
  textlint-rule-no-exclamation-question-mark \
  textlint-rule-no-empty-section \
  textlint-rule-date-weekday-mismatch \
  textlint-rule-terminology \
  textlint-rule-period-in-list-item \
  @textlint-rule/textlint-rule-no-invalid-control-character \
  @textlint-rule/textlint-rule-no-unmatched-pair \
  textlint-rule-footnote-order \
  textlint-rule-max-doc-width \
  textlint-rule-abbr-within-parentheses \
  textlint-rule-alex \
  textlint-rule-ginger \
  textlint-rule-write-good \
  textlint-rule-en-max-word-count \
  textlint-rule-apostrophe \
  textlint-rule-diacritics \
  textlint-rule-stop-words \
  textlint-rule-en-capitalization

RUN npm i -g --no-cache --ignore-scripts \
  prettier

ENV LC_ALL C.UTF-8

RUN usermod -l user ubuntu
# RUN useradd -ms /bin/bash user

USER user
WORKDIR /home/user/work

# Add ru locale to cspell as user
RUN cspell link add @cspell/dict-ru_ru

HEALTHCHECK NONE
