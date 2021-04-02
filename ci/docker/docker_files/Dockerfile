FROM hadolint/hadolint:v1.17.5-alpine AS hadolint_builder

FROM golang:1.13-alpine AS go_builder
LABEL Name=melg8/go_builder Version=0.0.5

ENV GO111MODULE on

RUN apk update \
 && apk add --no-cache \
    git=2.26.3-r0 \
    upx=3.96-r0

RUN go get github.com/talos-systems/conform/ \
 && upx --best --ultra-brute /go/bin/conform

RUN go get github.com/zricethezav/gitleaks/v7 \
 && upx --best --ultra-brute /go/bin/gitleaks

RUN go get github.com/github/git-sizer/ \
 && upx --best --ultra-brute /go/bin/git-sizer

FROM ruby:2.7.2-alpine3.13 AS ruby_builder

FROM alpine:3.11.5 AS edge_builder

RUN echo "https://uk.alpinelinux.org/alpine/edge/main/" >> \
    /etc/apk/repositories

RUN apk add --no-cache \
    clang-extra-tools=11.1.0-r0

FROM alpine:3.11.5 AS cit
LABEL Name=cit Version=0.0.5
ARG DEBIAN_FRONTEND=noninteractive

RUN apk add --no-cache \
    git=2.24.4-r0 \
    shellcheck=0.7.0-r1

COPY --from=ruby_builder /usr/ /usr/

RUN gem install mdl -v 0.11.0 \
 && gem install git-lint -v 1.4.0 \
 && sed -i 's.master.origin/main.g' \
 "/usr/local/lib/ruby/gems/2.7.0/gems/git-lint-1.4.0/lib/git/kit/repo.rb"\
 && sed -i 's.master.origin/main.g' \
 "/usr/local/lib/ruby/gems/2.7.0/gems/git-lint-1.4.0/lib/git\
/lint/branches/environments/local.rb" \
 && gem cleanup all \
 && rm -rf /usr/local/lib/gems/2.7.0/cache/*

RUN apk add --no-cache \
    python3=3.8.2-r2 \
 && pip3 install --no-cache-dir \
    codespell==1.17.1 \
    gitlint==0.15.0 \
    yamllint==1.24.2 \
    cmake_format==0.6.13

RUN apk add --no-cache \
    npm=12.21.0-r0 \
 && npm i -g --production \
    cspell@5.3.9 \
    conventional-changelog-conventionalcommits@4.3.1 \
    @commitlint/cli@12.0.1 \
    @commitlint/config-conventional@12.0.1 \
    dockerfile_lint \
    @ls-lint/ls-lint@1.9.2 \
    remark-cli@9.0.0 \
    remark-preset-lint-consistent@4.0.0 \
    remark-preset-lint-markdown-style-guide@4.0.0 \
    remark-preset-lint-recommended@5.0.0 \
    textlint@11.9.0 \
    textlint-rule-no-dead-link@4.7.0 \
    textlint-rule-no-start-duplicated-conjunction@2.0.2 \
    textlint-rule-max-comma@1.0.4 \
    textlint-rule-no-exclamation-question-mark@1.1.0 \
    textlint-rule-no-empty-section@1.1.0 \
    textlint-rule-date-weekday-mismatch@1.0.5 \
    textlint-rule-terminology@2.1.5 \
    textlint-rule-period-in-list-item@0.3.2 \
    @textlint-rule/textlint-rule-no-invalid-control-character@1.2.0 \
    @textlint-rule/textlint-rule-no-unmatched-pair@1.0.7 \
    textlint-rule-footnote-order@1.0.3 \
    textlint-rule-max-doc-width@1.4.0 \
    textlint-rule-abbr-within-parentheses@1.0.2 \
    textlint-rule-alex@3.0.0 \
    textlint-rule-ginger@2.2.1 \
    textlint-rule-write-good@1.6.2 \
    textlint-rule-en-max-word-count@1.1.0 \
    textlint-rule-apostrophe@2.0.0 \
    textlint-rule-diacritics@1.0.0 \
    textlint-rule-stop-words@2.0.9 \
    textlint-rule-en-capitalization@2.0.3 \
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
