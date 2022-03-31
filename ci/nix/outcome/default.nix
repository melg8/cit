{ lib, stdenv, fetchFromGitHub }:

stdenv.mkDerivation rec {
  pname = "outcome";
  version = "2.2.3";

  src = fetchFromGitHub {
    owner = "ned14";
    repo = "outcome";
    rev = "v${version}";
    sha256 = "1dfbfyncsmmhxqvf1224bgrflshznqhl1grkcvds7a25633iijg8";
  };

  installPhase = ''
    mkdir -p $out/include/boost
    cp ${src}/single-header/outcome.hpp $out/include/boost/outcome.hpp
  '';

  meta = with lib; {
    homepage = "https://github.com/gsl-lite/gsl-lite";
    description = ''
      A single-file header-only version of ISO C++ Guidelines Support Library
      (GSL) for C++98, C++11, and later'';
    platforms = platforms.all;
    license = licenses.mit;
  };
}
