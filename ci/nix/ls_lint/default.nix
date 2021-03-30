{ lib, buildGoPackage, fetchFromGitHub }:

buildGoPackage rec {
  pname = "ls-lint";
  version = "1.9.2";

  goPackagePath = "github.com/talos-systems/conform";
  #goDeps = ./deps.nix;

  src = fetchFromGitHub {
    owner = "loeffel-io";
    repo = pname;
    rev = "v${version}";
    sha256 = "0c2nzlbdirsa7w5zs06vlqsc6z2yi5ic3dzwad8shizxa4pv000";
  };

  meta = with lib; {
    description = "An extremely fast directory and filename linter - Bring some structure to your project directories";
    license = licenses.mit;
  };
}
