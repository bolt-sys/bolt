"""
Bazel extension for loading third-party dependencies.
"""

load("//:bazel/repositories.bzl", "source_dependencies")

def _non_module_dependencies_impl(_ctx):
    source_dependencies()

non_module_dependencies = module_extension(
    implementation = _non_module_dependencies_impl,
)
