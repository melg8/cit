{
  bootstrap-seeds = builtins.fetchGit rec {
    name = "bootstrap-seeds";
    url = "https://github.com/oriansj/${name}.git";
    ref = "refs/heads/master";
    rev = "6d3fb087efe2d7cc7938cf2aff0265c6bfc86370";
  };
  m2-planet = builtins.fetchGit rec {
    name = "M2-Planet";
    url = "https://github.com/oriansj/${name}.git";
    ref = "refs/heads/master";
    rev = "358b6cfb96e1685891a705a0bb31eda499d57974"; # Release 1.7.0
  };
  m2-libc =
    builtins.fetchGit rec {
      name = "M2libc";
      url = "https://github.com/oriansj/${name}.git";
      ref = "refs/heads/main";
      rev = "dd470ed9aea3ed00ce2924daaabe49311975903a"; # Release 1.7.0
    };
  mescc-tools = builtins.fetchGit rec {
    name = "mescc-tools";
    url = "https://github.com/oriansj/${name}.git";
    ref = "refs/heads/master";
    rev = "3f5b7588a20bc2225f25a31eed53a8152563955f"; # Use patched version with m2-planet 1.7.0
  };
  stage0-posix = builtins.fetchGit rec {
    name = "stage0-posix";
    url = "https://github.com/oriansj/${name}.git";
    ref = "refs/heads/master";
    rev = "0e6d8d6d3f261b550af1768b767f66b30fd07854";
  };
}
