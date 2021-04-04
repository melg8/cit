self: super: { ruby_2_7 = super.ruby_2_7.overrideAttrs (
        old: {
        prePatch = (old.prePatch or "") +
        ''
        chmod +w -R lib/rubygems/ext/
        '';
        patches = (old.patches or []) ++ [./ruby_2_7/mkmf_sort_headers.patch];
        }
    );
}