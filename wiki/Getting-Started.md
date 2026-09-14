# clangd setup

For anyone using `clangd`, for example with VSCode, you must generate a `compile_commands.json` file for features like autocomplete or symbol lookup to work. To do this we will use a tool called `compiledb`, which is a Python package. Assuming you have Python and `pip` installed, simply run `pip install compiledb` (`pip3` on some Linux distributions) to install the package, and then run:

```
compiledb make
```

in the root of this repository. clangd should now function. You may need to close/re-open files or even your editor.

These instructions were lifted from [this article](https://sethbarberee.github.io/lsp-and-pokemon-decomp), see there for further information.