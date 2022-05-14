{
  auto_injector = {
    dependencies = [ "marameters" "zeitwerk" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:1z10ph2ms6lidf5dfbsv0gfka0h32ywxyi7wkc0lc261f5aacisq";
      type = "gem";
    };
    version = "0.4.1";
  };
  cogger = {
    dependencies = [ "pastel" "zeitwerk" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:0f4w5dwpxbvh8fhka4bq23h69p1blc1vamcnkclbpi9x06pfv4lc";
      type = "gem";
    };
    version = "0.0.2";
  };
  concurrent-ruby = {
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:0s4fpn3mqiizpmpy2a24k4v365pv75y50292r8ajrv4i1p5b2k14";
      type = "gem";
    };
    version = "1.1.10";
  };
  dry-configurable = {
    dependencies = [ "concurrent-ruby" "dry-core" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:0750s6dmbmhxhfj6ws2x4nal9q0fwxwhvyzgk4zp9x90l70z48k2";
      type = "gem";
    };
    version = "0.15.0";
  };
  dry-container = {
    dependencies = [ "concurrent-ruby" "dry-configurable" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:059wgyhdaga895n8zkibksqyzcmkyg8n3lzy9dcmrl517r1n7in7";
      type = "gem";
    };
    version = "0.9.0";
  };
  dry-core = {
    dependencies = [ "concurrent-ruby" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:0cf1y6vcxg20s6lrwndvbbq7wdpr4fj18f1875ni16sbx4crv3x8";
      type = "gem";
    };
    version = "0.7.1";
  };
  dry-monads = {
    dependencies = [ "concurrent-ruby" "dry-core" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:0bncmcp9pvhwli9s95wh9k4w4dm0c1azwis0fynqg0b5k4wbyakw";
      type = "gem";
    };
    version = "1.4.0";
  };
  git-lint = {
    dependencies = [ "auto_injector" "cogger" "dry-container" "git_plus" "pastel" "refinements" "runcom" "spek" "zeitwerk" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:18jvv1wwy73ppdj0bfnyr8wimrr0dw5ahkq904p3810yv5c99hzy";
      type = "gem";
    };
    version = "4.0.1";
  };
  git_plus = {
    dependencies = [ "refinements" "zeitwerk" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:1y05abycv6nhijyps6lmrzc3r223mnkffi67y0idmp82yilzjcg9";
      type = "gem";
    };
    version = "1.3.0";
  };
  marameters = {
    dependencies = [ "refinements" "zeitwerk" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:0qw30idxw2sprfj3famjf4vfyh0qgc6xs398vx78z1b1f1rqlrsv";
      type = "gem";
    };
    version = "0.3.2";
  };
  pastel = {
    dependencies = [ "tty-color" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:0xash2gj08dfjvq4hy6l1z22s5v30fhizwgs10d6nviggpxsj7a8";
      type = "gem";
    };
    version = "0.8.0";
  };
  refinements = {
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:1rgw1p4gqxj4xjppaly000ivv2dlkifxkcg6psb4x9jm0j52jjmi";
      type = "gem";
    };
    version = "9.3.3";
  };
  runcom = {
    dependencies = [ "refinements" "xdg" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:1hv3hq4n80bhk52ci6sy41pi4dibzbb1j4khyf3n2masf74c3k03";
      type = "gem";
    };
    version = "8.3.3";
  };
  spek = {
    dependencies = [ "dry-monads" "refinements" "versionaire" "zeitwerk" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:1ffbbxwl79l56k4j48297c819kxidrka634y0krkskzd6432rwk9";
      type = "gem";
    };
    version = "0.2.2";
  };
  tty-color = {
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:0aik4kmhwwrmkysha7qibi2nyzb4c8kp42bd5vxnf8sf7b53g73g";
      type = "gem";
    };
    version = "0.6.0";
  };
  versionaire = {
    dependencies = [ "refinements" ];
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:0nqhgkxlyh6fsqjngjv6i25mhalls67vqyzcfr4hrlvkzbvbrzad";
      type = "gem";
    };
    version = "10.2.3";
  };
  xdg = {
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:1kqb1jmkc9q71756jqi8q5dkhmjr9idx34mqjlmp2wwdrl4cmhi2";
      type = "gem";
    };
    version = "6.3.3";
  };
  zeitwerk = {
    groups = [ "default" ];
    platforms = [ ];
    source = {
      remotes = [ "https://rubygems.org" ];
      sha256 = "sha256:09bq7j2p6mkbxnsg71s253dm2463kg51xc7bmjcxgyblqbh4ln7m";
      type = "gem";
    };
    version = "2.5.4";
  };
}
