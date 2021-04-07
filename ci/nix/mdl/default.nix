{ lib, bundlerApp, bundlerUpdateScript, ruby_3_0 }:

bundlerApp {
  pname = "mdl";
  gemdir = ./.;
  exes = [ "mdl" ];
  ruby = ruby_3_0;

  passthru.updateScript = bundlerUpdateScript "mdl";

  meta = with lib; {
    description = "A tool to check markdown files and flag style issues";
    homepage = "https://github.com/markdownlint/markdownlint";
    license = licenses.mit;
    platforms = platforms.all;
  };
}
