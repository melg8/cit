let
  nixpkgs = (builtins.fetchGit {
    name = "nixos-unstable-2021-04-09";
    url = "https://github.com/nixos/nixpkgs";
    ref = "refs/heads/nixos-unstable";
    rev = "9e377a6ce42dccd9b624ae4ce8f978dc892ba0e2";
  });
  pkgs = import nixpkgs { };
in
pkgs.mkShell rec {
  buildInputs = [
    (import ./ci/nix/tools.nix { inherit pkgs; })
  ];

  shellHook = ''
    for input in ${builtins.toString buildInputs}; do
       if [[ -e "$input/lib/node_modules" ]] ; then
           export NODE_PATH="$input/lib/node_modules:$NODE_PATH";
       fi
    done
  '';
}
