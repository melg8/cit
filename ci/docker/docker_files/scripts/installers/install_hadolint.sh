#!/bin/sh

set -e

curl -L https://get.haskellstack.org/ | sh
git clone https://github.com/hadolint/hadolint
cd hadolint
stack install --ghc-options="-fPIC"
curl -sSL https://github.com/upx/upx/releases/download/v3.94/upx-3.94-amd64_linux.tar.xz \
  | tar -x --xz --strip-components 1 upx-3.94-amd64_linux/upx \
  && ./upx --best --ultra-brute /root/.local/bin/hadolint
cp /root/.local/bin/hadolint /usr/local/bin/
rm -rf ../hadolint