{ lib, buildGoPackage, fetchFromGitHub }:

buildGoPackage rec {
  pname = "ls-lint";
  version = "1.9.2";

  goPackagePath = "github.com/loeffel-io/ls-lint/";
  goDeps = ./deps.nix;

  src = fetchFromGitHub {
    owner = "loeffel-io";
    repo = pname;
    rev = "v${version}";
    sha256 = "1wykmsycbkssvclqk4lfmdsmh83hzq34klbn0pdwr563vwh0p3v3";
  };

  meta = with lib; {
    description = "An extremely fast directory and filename linter - Bring some structure to your project directories";
    license = licenses.mit;
  };
}
