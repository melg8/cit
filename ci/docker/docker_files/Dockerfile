FROM ubuntu:18.04 AS hadolint_builder
LABEL Name=melg8/hadolint_builder Version=0.0.2

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

RUN apt-get update \
 && apt-get -y install --no-install-recommends \
    ca-certificates=* \
    curl=7.58.0-* \
    git=1:2.17.*

RUN curl -L https://get.haskellstack.org/ | sh

RUN git clone https://github.com/hadolint/hadolint

WORKDIR /hadolint

RUN stack install --ghc-options="-fPIC"

RUN curl -sSL https://github.com/upx/upx/releases/download/v3.94/upx-3.94-amd64_linux.tar.xz \
  | tar -x --xz --strip-components 1 upx-3.94-amd64_linux/upx \
 && ./upx --best --ultra-brute /root/.local/bin/hadolint \
 && rm -rf ../hadolint \
 && rm -rf /tmp/*

FROM golang:1.13-alpine AS go_builder
LABEL Name=melg8/go_builder Version=0.0.2

RUN apk update \
 && apk add --no-cache \
    upx=3.95-r2 \
 && export GO111MODULE=on \
 && go get github.com/talos-systems/conform \
 && go get github.com/zricethezav/gitleaks/v4 \
 && go get github.com/github/git-sizer \
 && go get github.com/jessfraz/dockfmt \
 && upx --best --ultra-brute /go/bin/conform \
 && upx --best --ultra-brute /go/bin/gitleaks \
 && upx --best --ultra-brute /go/bin/git-sizer \
 && upx --best --ultra-brute /go/bin/dockfmt

FROM ruby:2.7.1-alpine3.11 AS ruby_builder

FROM alpine:3.11.5 AS cit
LABEL Name=cit Version=0.0.2
ARG DEBIAN_FRONTEND=noninteractive

RUN apk add --no-cache \
    git=2.24.3-r0 \
    shellcheck=0.7.0-r1

COPY --from=ruby_builder /usr/ /usr/

RUN gem install mdl -v 0.9.0 \
 && gem install git-cop -v 4.1.0 \
 && gem cleanup all \
 && rm -rf /usr/local/lib/gems/2.7.0/cache/*

RUN apk add --no-cache \
    python3=3.8.2-r0 \
 && pip3 install --no-cache-dir \
    codespell==1.16.0 \
    gitlint==0.13.1 \
    yamllint==1.21.0

RUN apk add --no-cache \
    npm=12.15.0-r1 \
 && npm i -g --production \
    cspell@4.0.56 \
    conventional-changelog-conventionalcommits \
    @commitlint/cli \
    @ls-lint/ls-lint \
    remark-cli \
    remark-preset-lint-consistent \
    remark-preset-lint-markdown-style-guide \
    remark-preset-lint-recommended \
    dockerfile_lint \
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
    textlint-rule-en-capitalization \
    modclean \
 && modclean -r -n default:safe -D /usr/lib/node_modules \
 && npm rm modclean \
 && rm -rf root/.npm

COPY --from=hadolint_builder /root/.local/bin/hadolint /usr/local/bin/
COPY --from=go_builder /go/bin/ /usr/local/bin/

# Only for cpp code.

RUN apk add --no-cache \
    clang=9.0.0-r1 \
    g++=9.2.0-r4

RUN apk add --no-cache \
    cmake=3.15.5-r0 \
    make=4.2.1-r2

RUN pip3 install --no-cache-dir \
    cmake_format==0.6.10 \
    polysquare-cmake-linter==0.0.16

ENV LC_ALL C.UTF-8

RUN adduser -D -u 1000 user

USER user
WORKDIR /home/user/work

HEALTHCHECK NONE