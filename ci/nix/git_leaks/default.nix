{ lib, buildGoPackage, fetchFromGitHub }:

buildGoPackage rec {
  pname = "gitleaks";
  version = "7.3.0";

  goPackagePath = "github.com/zricethezav/gitleaks";
  goDeps = ./deps.nix;

  src = fetchFromGitHub {
    owner = "zricethezav";
    repo = pname;
    rev = "v${version}";
    sha256 = "0mxx3jmm2k9w6yjzsxnnp6k2w42k24zvnfcybppil8kfh6cax5i0";
  };

  meta = with lib; {
    description = "Policy enforcement for your pipelines.";
    license = licenses.mit;
  };
}
