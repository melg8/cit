json_drv:
let
  lib = import <nixpkgs/lib>;
  drvAttrs = (builtins.elemAt (builtins.attrValues json_drv) 0);
  envAttrs = drvAttrs."env";
  getAttr = set: name: builtins.replaceStrings [ "\n" ] [ "\\n" ]
    (toString set.${name});
  getEnvAttr = getAttr envAttrs;
  escapeArgs = str: lib.escape [ "`" ] (lib.escapeShellArg str);
  addToEnv = s: name: s + ''${name}=${getEnvAttr name}''\n'';
  env = builtins.foldl' addToEnv "" (builtins.attrNames envAttrs);
  getDrvAttr = getAttr drvAttrs;
  builder = getDrvAttr "builder";
  sanitizedBuilder =
    if builder == "builtin:fetchurl" then ''# "fixme: builtinfetchurl"''
    else builder;
  builderCall = "${sanitizedBuilder} ${getDrvAttr "args"}";
in
env + builderCall
