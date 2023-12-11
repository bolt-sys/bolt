# A simple cheat-sheet to provide a quick reference for the most common commands and operations.

## Table of Contents

### Code coverage

To generate a code coverage report, run the following command:

```shell
bazel coverage -c dbg --nocache_test_results --combined_report=lcov //...
```

To view the report

```shell
genhtml --branch-coverage --output genhtml "$(bazel info output_path)/_coverage/_coverage_report.dat"
```

references [bazel.build/configure/coverage](https://bazel.build/configure/coverage)
