FROM ubuntu:latest

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -y update \
 && apt-get -y install software-properties-common \
 && add-apt-repository -y ppa:apt-fast/stable \
 && DEBIAN_FRONTEND=noninteractive \
 && apt-get -y install apt-fast

RUN add-apt-repository ppa:ubuntu-toolchain-r/test \
 && add-apt-repository ppa:longsleep/golang-backports \
 && apt-fast update \
 && apt-fast -y dist-upgrade \
 && apt-fast -y install --no-install-recommends \
    build-essential \
    clang-9 \
    curl \
    gcc-9 \
    g++-9 \
    git \
    gnupg2 \
    golang-go \
    locales \
    nodejs \
    npm \
    python-pip \
    python-setuptools \
    ruby-full \
    shellcheck \
 && gem install mdl \
 && npm i -g npm@latest \
 && npm i -g cspell \
 && npm i conventional-changelog-conventionalcommits \
 && npm i -g @commitlint/cli \
 && npm i -g @ls-lint/ls-lint \
 && npm i -g remark-cli \
 && npm i remark-preset-lint-consistent \
 && npm i remark-preset-lint-markdown-style-guide \
 && npm i remark-preset-lint-recommended \
 && pip install codespell \
 && pip install gitlint \
 && pip install yamllint \
 && rm -rf /var/lib/apt/lists/*

RUN npm i -g textlint \
 && npm i textlint-rule-no-dead-link \
 && npm i textlint-rule-no-start-duplicated-conjunction \
 && npm i textlint-rule-max-comma \
 && npm i textlint-rule-no-exclamation-question-mark \
 && npm i textlint-rule-no-empty-section \
 && npm i textlint-rule-date-weekday-mismatch \
 && npm i textlint-rule-terminology \
 && npm i textlint-rule-period-in-list-item \
 && npm i @textlint-rule/textlint-rule-no-invalid-control-character \
 && npm i @textlint-rule/textlint-rule-no-unmatched-pair \
 && npm i textlint-rule-footnote-order \
 && npm i textlint-rule-max-doc-width \
 && npm i textlint-rule-abbr-within-parentheses \
 && npm i textlint-rule-alex \
 && npm i textlint-rule-ginger \
 && npm i textlint-rule-write-good \
 && npm i textlint-rule-en-max-word-count \
 && npm i textlint-rule-apostrophe \
 && npm i textlint-rule-diacritics \
 && npm i textlint-rule-stop-words \
 && npm i textlint-rule-en-capitalization

RUN curl -sSL https://rvm.io/mpapis.asc | gpg2 --import - \
 && curl -sSL https://rvm.io/pkuczynski.asc | gpg2 --import - \
 && curl -sSL https://get.rvm.io | bash -s stable \
 && /usr/local/rvm/bin/rvm install 2.7 \
 && /usr/local/rvm/gems/ruby-2.7.0/wrappers/gem install git-cop \
 && ln -s /usr/local/rvm/gems/ruby-2.7.0/wrappers/git-cop /usr/local/bin

RUN update-alternatives --install \
    /usr/bin/gcc gcc /usr/bin/gcc-9 60 --slave \
    /usr/bin/g++ g++ /usr/bin/g++-9 \
 && update-alternatives --install \
    /usr/bin/clang clang /usr/bin/clang-9 60 --slave \
    /usr/bin/clang++ clang++ /usr/bin/clang++-9

RUN export GO111MODULE=on \
 && go get github.com/talos-systems/conform \
 && go get github.com/zricethezav/gitleaks/v4@latest \
 && go get github.com/github/git-sizer \
 && cp ~/go/bin/* /usr/local/bin/

RUN apt-fast -y remove golang-go \
 && apt-fast -y autoremove \
 && apt-fast clean

ENV LC_ALL C.UTF-8

RUN useradd -u 1000 -m -s /bin/bash user

USER user
WORKDIR /home/user
