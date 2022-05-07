{ lib, callPackage, python39, python39Packages, fetchFromGitHub }:
let
  packages = python39Packages;
  packageOverrides = callPackage ./python-packages.nix { };
  python = python39.override { inherit packageOverrides; };
  python_with_packages = python.withPackages (ps: [ ps.pytablewriter ]);
in
packages.buildPythonApplication rec {
  pname = "mega_linter";
  version = "5.12.0";

  src = fetchFromGitHub {
    owner = "megalinter";
    repo = "megalinter";
    rev = "v${version}";
    sha256 = "1ndm9qwwji9haxvj84b314afcvjg6ssykdhjsrgldylr74rsgii8";
  };
  sourceRoot = "source/megalinter";
  doCheck = false;

  postPatch = ''
    substituteInPlace setup.py --replace \
      "install_requires=[" \
      "package_dir = {'megalinter': '.'},
       install_requires=["
  '';

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
    python.pkgs.pytablewriter
    terminaltables
    python.pkgs.giturlparse
    python.pkgs.multiprocessing-logging
    python.pkgs.mdx-truly-sane-lists
    python.pkgs.pychalk
    python.pkgs.webpreview
  ];

  postInstall = ''
    echo "Im here!"
    ls -la .
    echo "target!"

    mkdir $out/lib/python3.9/site-packages/megalinter/descriptors
    cp -r descriptors $out/lib/python3.9/site-packages/megalinter
    ls -la $out/lib/python3.9/site-packages/megalinter/descriptors
  '';


}
