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
  m2-libc = builtins.fetchGit rec {
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
  mescc-tools-extra = builtins.fetchGit rec {
    name = "mescc-tools-extra";
    url = "https://github.com/oriansj/${name}.git";
    ref = "refs/heads/master";
    rev = "5eadec73e8ea309a7a0a8e8409315c49209c1fb4";
  };
  stage0-posix = builtins.fetchGit rec {
    name = "stage0-posix";
    url = "https://github.com/oriansj/${name}.git";
    ref = "refs/heads/master";
    rev = "0e6d8d6d3f261b550af1768b767f66b30fd07854";
  };
  mes-m2 = builtins.fetchGit rec {
    name = "mes-m2";
    url = "https://github.com/oriansj/${name}.git";
    ref = "refs/heads/master";
    rev = "00e309856c4a505b7920956e64a20e9abb9475d0";
  };
  live-bootstrap = builtins.fetchGit rec {
    name = "live-bootstrap";
    url = "https://github.com/fosslinux/${name}.git";
    ref = "refs/heads/master";
    rev = "8210cc9e24b5495957a074f59a353ca68a1de0a0";
  };
  mes-wip-2_13 = builtins.fetchGit rec {
    name = "mes";
    url = "https://gitlab.com/janneke/${name}";
    ref = "refs/heads/wip-m2.13";
    rev = "2ab4c5c676cb66088b0fb8de03b40b01f07bd4e0";
  };
  nyacc = builtins.fetchGit rec {
    name = "nyacc";
    url = "https://git.savannah.gnu.org/git/${name}.git";
    ref = "refs/heads/rel-1.00";
    rev = "06a0985332cfbc76e759da50d228d5aa01b4f6a9";
  };

  toybox = builtins.fetchGit rec {
    name = "toybox";
    url = "https://github.com/landley/${name}.git";
    ref = "refs/heads/master";
    rev = "70754564fab010a9219735da231dc1e1e1aa7784";
  };
  customKaem = ./kaem.c;
  toyboxConfig = ./config.h;
}
