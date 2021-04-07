{ lib, bundlerApp, bundlerUpdateScript, ruby_3_0 }:

bundlerApp {
  pname = "git-lint";
  gemdir = ./.;
  exes = [ "git-lint" ];
  ruby = ruby_3_0;

  passthru.updateScript = bundlerUpdateScript "git-lint";

  meta = with lib; {
    description = "A command line interface for linting Git commits.";
    homepage = "https://github.com/bkuhlmann/git-lint";
    license = licenses.asl20;
    platforms = [ "x86_64-linux" ];
  };
}
