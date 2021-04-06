{ pkgs, stdenv, lib, fetchurl, cmake, glibc, nukeReferences, ... }:

stdenv.mkDerivation rec {
  pname = "how-to-use-pvs-studio-free";
  version = "2.0";

  src = fetchurl {
    url = "https://github.com/viva64/${pname}/archive/${version}.tar.gz";
    sha256 = "1a282msilc5pnxcbjkpc579mdrim6dcnwy58kxadjbyciyg3ry4v";
  };

  cmakeFlags = [ "-DPVS_STUDIO_SHARED=OFF" ];

  depsBuildBuild = [ cmake nukeReferences ];
  buildInputs = [ glibc.static ];

  preConfigure = ''
    substituteInPlace main.cpp \
      --replace "filesystem::" "std::experimental::filesystem::"

    substituteInPlace CMakeLists.txt \
      --replace "/usr" "$out"
  '';

  postFixup = ''
    nuke-refs $out/bin/${pname}
  '';

  meta = with lib; {
    description = "Pvs studio free usage helper";
    homepage = "https://github.com/viva64/${pname}";
    platforms = [ "x86_64-linux" ];
  };
}
