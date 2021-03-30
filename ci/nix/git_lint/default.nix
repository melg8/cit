{ lib, bundlerApp, bundlerUpdateScript, ruby_2_7}:

bundlerApp {
  pname = "git-lint";
  gemdir = ./.;
  exes = [ "git-lint" ];
  ruby = ruby_2_7;

  passthru.updateScript = bundlerUpdateScript "git-lint";

  meta = with lib; {
    description = "A command line interface for linting Git commits.";
    homepage = "https://github.com/bkuhlmann/git-lint";
    license = licenses.asl20;
    platforms = [ "x86_64-linux" ];
  };
}
