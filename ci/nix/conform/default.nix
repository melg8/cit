{ lib, buildGoPackage, fetchFromGitHub }:

buildGoPackage rec {
  pname = "conform";
  version = "0.1.0-alpha.20";

  goPackagePath = "github.com/talos-systems/conform";
  goDeps = ./deps.nix;

  src = fetchFromGitHub {
    owner = "talos-systems";
    repo = pname;
    rev = "refs/tags/v${version}";
    sha256 = "0c2nzlbdirsa7w5zs06vlqsc6z2yi5ic3dzwad8shizxa4pv3q2m";
  };

  meta = with lib; {
    description = "Policy enforcement for your pipelines.";
    license = licenses.mpl20;
  };
}
