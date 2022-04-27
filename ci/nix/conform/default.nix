# SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
#
# SPDX-License-Identifier: MIT

{ lib, buildGoModule, fetchFromGitHub, git }:

buildGoModule rec {
  pname = "conform";
  version = "0.1.0-alpha.25";

  src = fetchFromGitHub {
    owner = "talos-systems";
    repo = pname;
    rev = "refs/tags/v${version}";
    sha256 = "0v11z6xr16lp4wb68g4nn318arfkray8sjwbcc0q2gka1a5a01as";
  };

  nativeBuildInputs = [ git ];

  vendorSha256 = "18ggj4wskbprwmky5smfspabba0y9c3xginp890kiqhas3p2sa1s";

  meta = with lib; {
    description = "Policy enforcement for your pipelines.";
    license = licenses.mpl20;
  };
}
