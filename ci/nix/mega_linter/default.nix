# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

{ callPackage, runtimeShell, python39, python39Packages, fetchFromGitHub }:
let
  packages = python39Packages;
  packageOverrides = callPackage ./python-packages.nix { };
  python = python39.override { inherit packageOverrides; };
  custom_linters = callPackage ./custom_linters/default.nix { };
in
packages.buildPythonApplication rec {
  pname = "mega_linter";
  version = "5.12.0";

  src = fetchFromGitHub {
    owner = "megalinter";
    repo = "megalinter";
    rev = "v${version}";
    sha256 = "sha256:1ndm9qwwji9haxvj84b314afcvjg6ssykdhjsrgldylr74rsgii8";
  };
  sourceRoot = "source/megalinter";
  doCheck = false;

  patches = [
    # Make possible to build from megalinter subdirectory.
    ./patches/setup.patch

    # Add cit flavor.
    ./patches/all_flavors.patch

    # Add cit flavor to schema.
    ./patches/json_schema.patch

    # Add cit flavor description to flavor factory.
    ./patches/flavor_factory.patch

    # Use fire icon when error occurs.
    ./patches/icons.patch

    # Make compatible with nixos. Search for bash.
    ./patches/bin_bash.patch
  ];

  pythonPath = with packages; [ setuptools ];

  propagatedBuildInputs = with packages; [
    pytest-cov
    pytest-timeout
    beautifulsoup4
    jsonschema
    mkdocs-material
    PyGithub
    GitPython
    python-gitlab
    yq
    commentjson
    jsonpickle
    pytest
    markdown
    terminaltables

    # Not packaged by nixpkgs.
    python.pkgs.pytablewriter
    python.pkgs.giturlparse
    python.pkgs.multiprocessing-logging
    python.pkgs.mdx-truly-sane-lists
    python.pkgs.pychalk
    python.pkgs.webpreview
  ];

  postInstall = ''
    # Install default descriptors.
    mkdir $out/lib/python3.9/site-packages/megalinter/descriptors
    cp -r descriptors $out/lib/python3.9/site-packages/megalinter

    # Copy custom descriptors.
    cp -r ${custom_linters}/* $out/lib/python3.9/site-packages/megalinter

    # Enable usage of megalinter as executable script form bin.
    mkdir -p $out/bin
    cat << EOF > $out/bin/megalinter
    #!${runtimeShell}
    PYTHONPATH="$PYTHONPATH" exec ${python}/bin/python -m megalinter.run "$@"
    EOF
    chmod +x $out/bin/megalinter
  '';
}
