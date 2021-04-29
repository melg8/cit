let
  rev = "85aef7706e35226d572b419971a98cf0d2f22ae6";
in
builtins.fetchTarball {
  url = "https://github.com/NixOS/nixpkgs/archive/${rev}.tar.gz";
  sha256 = "1s229hiq2kxki0xw8dfbs6gw0qr0wschwq4wmy1mfjrwcgpnciba";
}
