# Cheat-Sheet for Common Commands and Operations

This cheat-sheet provides a quick reference for the most common commands and operations.

## Table of Contents

- [Generating compile-commands.json](#generating-compile-commandsjson)
- [Generating and Viewing Code Coverage](#generating-and-viewing-code-coverage)

### Generating compile-commands.json

The `compile-commands.json` file is used by the `clangd` extension in VSCode to provide intellisense. You may need to generate this file if you're using `clangd`. 

To generate a `compile-commands.json` file, run the following command in your shell:

```shell
bazel run //:refresh_compile_commands
```

### Generating and Viewing Code Coverage

To generate a code coverage report, run the following command:

```shell
bazel coverage -c dbg --nocache_test_results --combined_report=lcov //lib/runtime:all
```

After generating the report, you can view it by running:

```shell
genhtml --branch-coverage --output genhtml "$(bazel info output_path)/_coverage/_coverage_report.dat"
```

For more details on code coverage with Bazel, visit [bazel.build/configure/coverage](https://bazel.build/configure/coverage)
