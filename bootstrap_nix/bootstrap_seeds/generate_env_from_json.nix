json_drv:
let
  drvAttrs = (builtins.elemAt (builtins.attrValues json_drv) 0);
  envAttrs = drvAttrs."env";
  getAttr = set: name: builtins.replaceStrings [ "\n" ] [ "\\n" ]
    (toString set.${name});
  getEnvAttr = getAttr envAttrs;
  addToEnv = s: name: s + ''${name}="${getEnvAttr name} ''\n"'';
  env = builtins.foldl' addToEnv "" (builtins.attrNames envAttrs);
  getDrvAttr = getAttr drvAttrs;
  builderCall = "${getDrvAttr "builder"} ${getDrvAttr "args"}";
in
env + builderCall
