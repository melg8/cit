#!/bin/sh

set -e

rm -rf /tmp/*
rm root/go/pkg/mod/golang.org/x/\
oauth2@v0.0.0-20190604053449-0f29369cfe45/\
google/testdata/gcloud/credentials
rm root/go/pkg/mod/golang.org/x/sys@v0.0.0-20190726091711-fc99dfbffb4e/unix/linux/Dockerfile
rm root/go/pkg/mod/github.com/zricethezav/gitleaks/v4@v4.1.1/Dockerfile
rm root/go/pkg/mod/golang.org/x/net@v0.0.0-20190724013045-ca1201d0de80/http2/Dockerfile
rm root/go/pkg/mod/github.com/zricethezav/gitleaks@v1.25.1/Dockerfile
rm usr/local/lib/node_modules/dockerfile_lint/Dockerfile
rm root/go/pkg/mod/github.com/moby/buildkit@v0.0.0-20180717184648-628681f8e4aa/frontend/dockerfile/parser/testfiles-negative/env_no_value/Dockerfile
rm root/go/pkg/mod/github.com/moby/buildkit@v0.0.0-20180717184648-628681f8e4aa/frontend/dockerfile/parser/testfile-line/Dockerfile
rm root/go/pkg/mod/github.com/moby/buildkit@v0.0.0-20180717184648-628681f8e4aa/frontend/dockerfile/parser/testfiles-negative/shykes-nested-json/Dockerfile
rm root/go/pkg/mod/github.com/jessfraz/dockfmt@v0.3.3/Dockerfile
rm root/go/pkg/mod/github.com/moby/buildkit@v0.0.0-20180717184648-628681f8e4aa/frontend/dockerfile/cmd/dockerfile-frontend/Dockerfile
rm root/go/pkg/mod/golang.org/x/sys@v0.0.0-20180909124046-d0be0721c37e/unix/linux/Dockerfile
rm root/go/pkg/mod/golang.org/x/net@v0.0.0-20200301022130-244492dfa37a/http2/Dockerfile
rm root/go/pkg/mod/golang.org/x/sys@v0.0.0-20200302150141-5c8b2ff67527/unix/linux/Dockerfile
rm root/go/pkg/mod/github.com/golangci/misspell@v0.3.4/Dockerfile
rm root/go/pkg/mod/github.com/talos-systems/conform@v0.1.0-alpha.19/Dockerfile
rm -rf root/go/pkg/mod/github.com/moby/buildkit@v0.0.0-20180717184648-628681f8e4aa/frontend/dockerfile/parser/testfiles/*
rm -rf root/.npm
