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
  builder = getDrvAttr "builder";
  builderCall = "${builder} ${getDrvAttr "args"}";
  firstBuilderRe = ".*kaem-optional-seed";
in
if builtins.isList (builtins.match firstBuilderRe builder)
  then builderCall else env + builderCall
