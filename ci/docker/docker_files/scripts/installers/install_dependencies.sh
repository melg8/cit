#!/bin/sh

set -e

apt-get -y update
apt-get -y install --no-install-recommends \
software-properties-common=0.96.*

add-apt-repository -y ppa:apt-fast/stable
apt-get -y update
apt-get -y install --no-install-recommends \
apt-fast=1.9.*
add-apt-repository -y ppa:ubuntu-toolchain-r/test
add-apt-repository -y ppa:longsleep/golang-backports

apt-fast -y install --no-install-recommends \
    build-essential=12.4* \
    clang-9=1:9-* \
    curl=7.58.0-* \
    gcc-9=9.2.1-* \
    g++-9=9.2.1-* \
    git=1:2.* \
    gnupg2=2.2.4* \
    locales=2.27-* \
    nodejs=8.10.0* \
    npm=3.5.2-* \
    python-pip=9.0.1-* \
    python-setuptools=39.0.1-* \
    ruby-full=1:2.5.1 \
    shellcheck=0.4.6-1



pip install codespell==1.16.0
pip install gitlint==0.13.1
pip install yamllint==1.21.0


npm i -g npm@6.14.4

npm i -g cspell

npm i -g --save conventional-changelog-conventionalcommits
npm i -g --save @commitlint/cli@next

npm i -g @ls-lint/ls-lint

npm i -g remark-cli
npm i -g remark-preset-lint-consistent
npm i -g remark-preset-lint-markdown-style-guide
npm i -g remark-preset-lint-recommended

npm i -g dockerfile_lint

update-alternatives --install \
/usr/bin/gcc gcc /usr/bin/gcc-9 60 --slave \
/usr/bin/g++ g++ /usr/bin/g++-9

update-alternatives --install \
    /usr/bin/clang clang /usr/bin/clang-9 60 --slave \
    /usr/bin/clang++ clang++ /usr/bin/clang++-9

gem install mdl -v 0.9.0

curl -sSL https://rvm.io/mpapis.asc | gpg2 --import -
curl -sSL https://rvm.io/pkuczynski.asc | gpg2 --import -
curl -sSL https://get.rvm.io | bash -s stable
/home/travis/.rvm/bin/rvm install 2.7
/home/travis/.rvm/gems/ruby-2.7.0/wrappers/gem install git-cop -v 4.1.0
ln -s /home/travis/.rvm/gems/ruby-2.7.0/wrappers/git-cop /usr/local/bin

eval "$(curl -sL https://raw.githubusercontent.com/travis-ci/gimme/master/gimme | GIMME_GO_VERSION=1.14 bash)"
export GO111MODULE=on
go get github.com/talos-systems/conform
go get github.com/zricethezav/gitleaks/v4
go get github.com/github/git-sizer
go get github.com/jessfraz/dockfmt


npm i -g textlint
npm i -g textlint-rule-no-dead-link
npm i -g textlint-rule-no-start-duplicated-conjunction
npm i -g textlint-rule-max-comma
npm i -g textlint-rule-no-exclamation-question-mark
npm i -g textlint-rule-no-empty-section
npm i -g textlint-rule-date-weekday-mismatch
npm i -g textlint-rule-terminology
npm i -g textlint-rule-period-in-list-item
npm i -g @textlint-rule/textlint-rule-no-invalid-control-character
npm i -g @textlint-rule/textlint-rule-no-unmatched-pair
npm i -g textlint-rule-footnote-order
npm i -g textlint-rule-max-doc-width
npm i -g textlint-rule-abbr-within-parentheses
npm i -g textlint-rule-alex
npm i -g textlint-rule-ginger
npm i -g textlint-rule-write-good
npm i -g textlint-rule-en-max-word-count
npm i -g textlint-rule-apostrophe
npm i -g textlint-rule-diacritics
npm i -g textlint-rule-stop-words
npm i -g textlint-rule-en-capitalization
