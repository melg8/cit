{ lib, mkYarnPackage, makeWrapper, nodePackages, nodejs }:

mkYarnPackage rec {
  pname = "commitlint";
  version = "12.0.1";

  src = ./.;

  nativeBuildInputs = [ nodePackages.typescript makeWrapper ];
  packageJSON = ./package.json;
  yarnLock = ./yarn.lock;
  yarnNix = ./yarn_deps.nix;

  buildPhase = ''
    mkdir -p $out/modules
    cp -r node_modules $out/modules
  '';

  postInstall = ''
    OUT_JS_DIR="$out/modules/node_modules/@commitlint/cli"
    makeWrapper '${nodejs}/bin/node' "$out/bin/commitlint" \
      --add-flags "$OUT_JS_DIR/cli.js" --set NODE_PATH $out/modules/node_modules:$out/modules/node_modules/@commitlint
  '';

  meta = {
    description = "Lint commit messages";
    homepage = "https://github.com/conventional-changelog/commitlint";
    license = lib.licenses.mit;
  };
}
