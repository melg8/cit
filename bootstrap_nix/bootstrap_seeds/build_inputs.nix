{ drv_path, visited ? [ ], lib ? import <nixpkgs/lib> }:
let
  drv_direct_dependencies =
    drv_path:
    let
      drv_file = builtins.readFile drv_path;
      storeDirRe = lib.replaceStrings [ "." ] [ "\\." ] builtins.storeDir;
      storeBaseRe = "[0-9a-df-np-sv-z]{32}-[+_?=a-zA-Z0-9-][+_?=.a-zA-Z0-9-]*";
      re = "(${storeDirRe}/${storeBaseRe}\\.drv)";
      inputs = lib.concatLists (lib.filter lib.isList (builtins.split re drv_file));
    in
    inputs;

  drv_all_dependencies = { drv_path, visited }:
    let
      direct_dependencies = drv_direct_dependencies drv_path;
      minimal_path = { paths = [ drv_path ]; visited = visited ++ [ drv_path ]; };
      just_visited = { paths = [ ]; visited = lib.unique (visited ++ [ drv_path ]); };
      concat_outs = out1: out2: {
        paths = out1.paths ++ out2.paths;
        visited = lib.unique (out1.visited ++ out2.visited);
      };
      visit_all_childs = (out: dependency:
        if (builtins.elem dependency out.visited) then out
        else concat_outs out (drv_all_dependencies { drv_path = dependency; visited = out.visited; }));
      result =
        if (builtins.length direct_dependencies) == 0 then minimal_path
        else concat_outs (builtins.foldl' visit_all_childs just_visited direct_dependencies) minimal_path;
    in
    result;
in
(drv_all_dependencies { drv_path = drv_path; visited = visited; }).paths
