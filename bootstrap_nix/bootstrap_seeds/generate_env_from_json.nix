let
  getAttr = set: name: builtins.replaceStrings [ "\n" ] [ "\\n" ]
    (toString set.${name});
  drvAttrs = jsonDrv: (builtins.elemAt (builtins.attrValues jsonDrv) 0);
  envAttrs = jsonDrv: (drvAttrs jsonDrv)."env";
  getEnvAttr = jsonDrv: getAttr (envAttrs jsonDrv);

  advancedKaemEnv = jsonDrv: name: ''${name}=${getEnvAttr jsonDrv name}''\n'';

  generateEnvFromJson =
    envCapture: jsonDrv:
    let
      lib = import <nixpkgs/lib>;
      addToEnv = s: name: s + (envCapture jsonDrv name);

      env = builtins.foldl' addToEnv "" (builtins.attrNames (envAttrs jsonDrv));
      getDrvAttr = getAttr (drvAttrs jsonDrv);
      builder = getDrvAttr "builder";
      sanitizedBuilder =
        if builder == "builtin:fetchurl" then ''# "fixme: builtinfetchurl"''
        else builder;
      builderCall = "${sanitizedBuilder} ${getDrvAttr "args"}";
    in
    env + builderCall;
in
{
  generateLocalKaemEnv = generateEnvFromJson advancedKaemEnv;
  generateAdvancedKaemEnv = generateEnvFromJson advancedKaemEnv;
}
