load("@io_bazel_stardoc//stardoc:stardoc.bzl", "stardoc")
load("@bazel_skylib//:bzl_library.bzl", "bzl_library")

bzl_library(
    name = "image_rules",
    srcs = ["rules.bzl"],
)

stardoc(
    name = "image_rules_docs",
    out = "image_rules_docs.md",
    input = ":image_rules",
)
