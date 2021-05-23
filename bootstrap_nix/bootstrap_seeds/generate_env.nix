drv:
let
  drvAttrs = builtins.removeAttrs drv.drvAttrs [ "args" ] // {
    out = toString drv.out;
    _args = toString drv.args;
  };
  getDrvAttr = name: builtins.replaceStrings [ "\n" ] [ "\\n" ]
    (toString drvAttrs.${name});
  addToEnv = s: name: s + ''${name}="${getDrvAttr name} ''\n"'';
  env = builtins.foldl' addToEnv "" (builtins.attrNames drvAttrs);
  builderCall = "${getDrvAttr "builder"} ${getDrvAttr "_args"}";
in
env + builderCall
