{ lib, buildGoModule, fetchFromGitHub}:

buildGoModule rec {
  pname = "ls-lint";
  version = "1.11.0";

  src = fetchFromGitHub {
    owner = "loeffel-io";
    repo = pname;
    rev = "v${version}";
    sha256 = "1a9i4wdqvqaz8a0wd1yz7p8f8jdgrwm23f7pq3kflwg1s7086bpn";
  };

  vendorSha256 = "1wnwr6nzgpr6yr9v269ybmq0nfhfv681l7bkl1ilzb6p6yvjj1kz";

  meta = with lib; {
    description = "An extremely fast directory and filename linter - Bring some structure to your project directories";
    license = licenses.mit;
  };
}
