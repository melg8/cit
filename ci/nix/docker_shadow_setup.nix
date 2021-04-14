{ pkgs }: { user, uid, gid ? uid }: with pkgs; [
  (
    writeTextDir "etc/shadow" ''
      root:!x:::::::
      ${user}:!:::::::
    ''
  )
  (
    writeTextDir "etc/passwd" ''
      root:x:0:0::/root:${runtimeShell}
      ${user}:x:${toString uid}:${toString gid}::/home/${user}:
    ''
  )
  (
    writeTextDir "etc/group" ''
      root:x:0:
      ${user}:x:${toString gid}:
    ''
  )
  (
    writeTextDir "etc/gshadow" ''
      root:x::
      ${user}:x::
    ''
  )
]
