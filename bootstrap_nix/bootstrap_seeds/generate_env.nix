# "all"
# "args"
# "drvAttrs"
# "drvPath"
# "outPath"
# "outputName"
# "type"
let
  drv = mes-m2-with-tools;
in
builtins.removeAttrs drv.drvAttrs [ "args" ] // { out = toString drv.out; }

  env: MyCustomVariable=someValue for it
MyFirstCustomVariable=10
MySecondCustomVariable=this
is
multiline

allowedReferences=
allowedRequisites=
builder=/nix/store/v00nmlmnj0qsdc6nprm68hxq4p6hfygp-bootstrap-seeds/POSIX/x86/kaem-optional-seed
ca_test_strange_variable_at_end=zzz
name=mes-m2-with-tools
out=/nix/store/83svikld3l9all0ynr2lpm0wis2w9zgh-mes-m2-with-tools
outputs=out
srcs=/nix/store/m9kwz6c1sav8s9clsk2mr1jzkmk5svlh-kaem.run
system=x86_64-linux
