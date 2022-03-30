{ lib, stdenv, fetchFromGitHub, cmake }:

stdenv.mkDerivation rec {
  pname = "gsl_lite";
  version = "0.40.0";

  src = fetchFromGitHub {
    owner = "gsl-lite";
    repo = "gsl-lite";
    rev = "v${version}";
    sha256 = "0n86d0xlmblv69rd20p768a3xz8gk6hq1dxy6b5n57y5qm7jqjgk";
  };

  nativeBuildInputs = [ cmake ];

  doCheck = true;

  meta = with lib; {
    homepage = "https://github.com/gsl-lite/gsl-lite";
    description =
      ''A single-file header-only version of ISO C++
        Guidelines Support Library  (GSL) for C++98, C++11, and later'';
    platforms = platforms.all;
    license = licenses.mit;
  };
}
