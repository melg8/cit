{ lib, python38Packages, arrow-cpp, git }:
let
  sh_1_14_1 = python38Packages.sh.override (old:
    {
      buildPythonPackage = attrs: old.buildPythonPackage (attrs // {
        version = "1.14.1";
        src = old.fetchPypi {
          pname = "sh";
          version = "1.14.1";
          sha256 = "13hxgifab9ichla13qaa7sh8r0il7vs1r21js72s0n355zr9mair";
        };
      }
      );
    }
  );
  arrow_0_17_0 = python38Packages.arrow.override (old:
    {
      buildPythonPackage = attrs: old.buildPythonPackage (attrs // {
        version = "0.17.0";
        src = old.fetchPypi {
          pname = "arrow";
          version = "0.17.0";
          sha256 = "1m3fpz96w3g08i9x9cpqh3cr795y9zbj1bfnay3ccdhxv86d227z";
        };
      }
      );
    }
  );
in
python38Packages.buildPythonApplication rec {
  pname = "gitlint";
  version = "0.15.0";

  src = python38Packages.fetchPypi {
    inherit pname version;
    sha256 = "0xkp9wpyids6b28mrpaif85g7ivfzwx2v3rr87ciia35sxkfkrxa";
  };

  propagatedBuildInputs = [ git arrow_0_17_0 python38Packages.click sh_1_14_1 ];

  checkInputs = [
    git
    sh_1_14_1
  ];

  meta = {
    description = "Linting for your git commit messages ";
    homepage = "https://github.com/jorisroovers/gitlint";
    license = lib.licenses.mit;
    platforms = lib.platforms.all;
  };
}
