{ lib, python39Packages, arrow-cpp, git }:
let
  arrow_1_2_0 = python39Packages.arrow.override (old:
    {
      buildPythonPackage = attrs: old.buildPythonPackage (attrs // {
        version = "1.2.0";
        src = old.fetchPypi {
          pname = "arrow";
          version = "1.2.0";
          sha256 = "0x70a057dqki2z1ny491ixbg980hg4lihc7g1zmy69g4v6xjkz0n";
        };
      }
      );
    }
  );
  click_8_0_1 = python39Packages.click.override (old:
    {
      buildPythonPackage = attrs: old.buildPythonPackage (attrs // {
        version = "8.0.1";
        src = old.fetchPypi {
          pname = "click";
          version = "8.0.1";
          sha256 = "0ymdyf37acq4qxh038q0xx44qgj6y2kf0jd0ivvix6qij88w214c";
        };
      }
      );
    }
  );
in
python39Packages.buildPythonApplication rec {
  pname = "gitlint";
  version = "0.16.0";

  src = python39Packages.fetchPypi {
    inherit pname version;
    sha256 = "0nzm6wxz3dzyz8hk07by6fzg0jy1myhmssrjyrnzdfqiwvd2pvih";
  };

  propagatedBuildInputs = [ git arrow_1_2_0 click_8_0_1 python39Packages.sh ];

  checkInputs = [
    git
    python39Packages.sh
  ];

  meta = {
    description = "Linting for your git commit messages ";
    homepage = "https://github.com/jorisroovers/gitlint";
    license = lib.licenses.mit;
    platforms = lib.platforms.all;
  };
}
