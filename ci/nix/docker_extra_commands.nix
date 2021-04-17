{ contents }:
''
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

  mkdir -p nix/store
  chmod 1777 nix/store

  mkdir -p nix/store/.links
  chmod 1777 nix/store/.links
  mkdir -p nix/var
  chmod 1777 nix/var
  mkdir -p home/user/.cache
  chmod 1777 home/user/.cache
  mkdir -p home/user/.local
  chmod 1777 home/user/.local

  chmod +w etc/ssl/certs
  cp etc/ssl/certs/ca-bundle.crt etc/ssl/certs/ca-certificates.crt
  chmod -w etc/ssl/certs

  if [[ -e lib ]] ; then
    cp -al lib usr/lib
  fi
''
