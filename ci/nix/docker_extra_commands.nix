{ nixpkgs, pkgs, contents }:
let
  loadNixDb = contents:
    let
      contentsList = if builtins.isList contents then contents else [ contents ];
    in
    with pkgs; ''
      export NIX_REMOTE=local?root=$PWD
      export USER=nobody
      ${libfaketime}/bin/faketime -f "1970-01-01 00:00:01" \
      ${nix}/bin/nix-store --load-db < ${closureInfo { rootPaths = contentsList; }}/registration

      mkdir -p nix/var/nix/gcroots/docker/
      for i in ${lib.concatStringsSep " " contentsList}; do
      ln -s $i nix/var/nix/gcroots/docker/$(basename $i)
      done;

      # Because per-user is filled depending on host system.
      rm -rf nix/var/nix/gcroots/per-user
      mkdir -p nix/var/nix/gcroots/per-user/user

      rm -rf nix/var/nix/profiles/per-user
      mkdir -p nix/var/nix/profiles/per-user/user

      # Because of bugged link to non existent location which differs inside
      # of docker.
      rm nix/var/nix/gcroots/profiles
    '';
in
(loadNixDb (contents ++ [ nixpkgs ])) + ''
  mkdir -p home/user/work
  mkdir -p usr/bin
  ln -s /bin/env usr/bin/env

  for path in ${builtins.toString contents}; do
          echo "Linking path: $path"
          if [[ -e "$path/bin" ]] ; then
          for file in $(ls $path/bin/); do
              if [[ ! -e bin/$file ]]; then
                echo "Linking file: $path/bin/$file -> bin/$file"
                if [[ -L bin/$file ]]; then
                    ln -sfn $path/bin/$file usr/bin/$file
                else
                    ln -sfn $path/bin/$file bin/$file
                fi

              fi
          done
          fi
        done
  mkdir -p tmp
  chmod 1777 tmp

  mkdir -vp root
  chmod +w etc/ssl/certs
  cp etc/ssl/certs/ca-bundle.crt etc/ssl/certs/ca-certificates.crt
  chmod -w etc/ssl/certs

  if [[ -e lib ]] ; then
    cp -al lib usr/lib
  fi
''
