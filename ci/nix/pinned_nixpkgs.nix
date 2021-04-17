let
  rev = "9e377a6ce42dccd9b624ae4ce8f978dc892ba0e2";
in
builtins.fetchTarball {
  url = "https://github.com/NixOS/nixpkgs/archive/${rev}.tar.gz";
  sha256 = "1r3ll77hyqn28d9i4cf3vqd9v48fmaa1j8ps8c4fm4f8gqf4kpl1";
}
