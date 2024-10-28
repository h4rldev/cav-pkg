{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  allowUnfree = true;
  name = "cav";
  packages = with pkgs; [
    clang-tools
    gcc
    bash-language-server
    valgrind
    yamlfix
    yamlfmt
    alejandra
    cbfmt
    mdformat
    beautysh
    curl
    shfmt
    uncrustify
    tree-sitter-grammars.tree-sitter-c
    gdb
  ];
  shellHook = ''
  '';
}
