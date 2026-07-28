{
  description = "muopt";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs =
    inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      imports = [
        inputs.flake-parts.flakeModules.easyOverlay
      ];

      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      perSystem =
        { pkgs, ... }:
        let
          version = "0.1.0";

          muopt = pkgs.stdenv.mkDerivation {
            pname = "muopt";
            inherit version;
            src = ./.;

            nativeBuildInputs = [ pkgs.cmake ];
            cmakeFlags = [
              "-DMUOPT_BUILD_EXAMPLES=OFF"
              "-DMUOPT_BUILD_TESTS=OFF"
              "-DMUOPT_BUILD_FUZZ=OFF"
            ];

            meta = {
              description = "A header-only C++17 micro-library for argument parsing";
              homepage = "https://github.com/secona/muopt";
              license = pkgs.lib.licenses.mit;
              platforms = pkgs.lib.platforms.all;
            };
          };
        in
        {
          overlayAttrs = { inherit muopt; };

          packages.default = muopt;

          devShells.default =
            pkgs.mkShell.override
              {
                stdenv = pkgs.clangStdenv;
              }
              {
                packages = with pkgs; [
                  clang-tools
                  cmake
                  ninja
                ];
              };
        };
    };
}
