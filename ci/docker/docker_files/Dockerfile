FROM hadolint/hadolint:v1.17.5-alpine AS hadolint_builder

FROM golang:1.13-alpine AS go_builder
LABEL Name=melg8/go_builder Version=0.0.4

ENV GO111MODULE on

RUN apk update \
 && apk add --no-cache \
    git=2.26.3-r0 \
    upx=3.96-r0

RUN go get github.com/talos-systems/conform \
 && upx --best --ultra-brute /go/bin/conform

RUN go get github.com/zricethezav/gitleaks/v7 \
 && upx --best --ultra-brute /go/bin/gitleaks

RUN go get github.com/github/git-sizer \
 && upx --best --ultra-brute /go/bin/git-sizer

FROM ruby:3.0.0-alpine3.13 AS ruby_builder

FROM alpine:3.11.5 AS edge_builder

RUN echo "https://uk.alpinelinux.org/alpine/edge/main/" >> \
    /etc/apk/repositories

RUN apk add --no-cache \
    clang-extra-tools=11.1.0-r0

FROM alpine:3.11.5 AS cit
LABEL Name=cit Version=0.0.4
ARG DEBIAN_FRONTEND=noninteractive

RUN apk add --no-cache \
    git=2.24.4-r0 \
    shellcheck=0.7.0-r1

COPY --from=ruby_builder /usr/ /usr/

RUN gem install mdl -v 0.9.0 \
 && gem install git-lint -v 2.1.0 \
 && gem cleanup all \
 && rm -rf /usr/local/lib/gems/2.7.0/cache/*

RUN apk add --no-cache \
    python3=3.8.2-r2 \
 && pip3 install --no-cache-dir \
    codespell==1.16.0 \
    gitlint==0.13.1 \
    yamllint==1.21.0 \
    cmake_format==0.6.10

RUN apk add --no-cache \
    npm=12.21.0-r0 \
 && npm i -g --production \
    cspell@4.0.56 \
    conventional-changelog-conventionalcommits \
    @commitlint/cli \
    @commitlint/config-conventional \
    dockerfile_lint \
    @ls-lint/ls-lint \
    remark-cli \
    remark-preset-lint-consistent \
    remark-preset-lint-markdown-style-guide \
    remark-preset-lint-recommended \
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

COPY --from=hadolint_builder /bin/hadolint /usr/local/bin/
COPY --from=go_builder /go/bin/ /usr/local/bin/
COPY --from=edge_builder /usr/lib/libffi.so.7 /usr/lib/
COPY --from=edge_builder /usr/lib/libLLVM-11.so /usr/lib/
COPY --from=edge_builder /usr/lib/llvm11 /usr/lib/
COPY --from=edge_builder /usr/bin/clang-tidy /usr/bin/

# Only for cpp code.
RUN apk add --no-cache \
    clang=9.0.0-r1 \
    g++=9.3.0-r0

RUN apk add --no-cache \
    ccache=3.7.6-r0 \
    cmake=3.15.5-r0 \
    make=4.2.1-r2

ENV LC_ALL C.UTF-8

RUN adduser -D -u 1000 user

USER user
WORKDIR /home/user/work

HEALTHCHECK NONE
