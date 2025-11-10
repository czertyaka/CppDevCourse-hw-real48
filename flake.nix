{
  description = "Dev shells to build this project";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-25.05";
  };

  outputs = { self, nixpkgs }:
    let
      pkgs = import nixpkgs { system = "x86_64-linux"; };
    in {
      devShells.x86_64-linux = {

        # shell for native build
        native = pkgs.mkShell {
          buildInputs = with pkgs; [
            cmake
            cppcheck
            gnumake
            gtest
            llvmPackages_20.clang
            llvmPackages_20.clang-tools
            llvmPackages_20.lld
          ];
          shellHook = ''
            export CC=${pkgs.llvmPackages_20.clang}/bin/clang
            export CXX=${pkgs.llvmPackages_20.clang}/bin/clang++
            export LD=${pkgs.llvmPackages_20.lld}/bin/lld
          '';
        };
        default = self.devShells.x86_64-linux.native;
      };
    };
}
