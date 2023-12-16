load("@rules_foreign_cc//foreign_cc:defs.bzl", "configure_make")

package(default_visibility = ["//visibility:public"])

exports_files(["**/*"])

filegroup(
    name = "srcs",
    srcs = glob(["**/*"]),
)

configure_make(
    name = "bin",
    lib_source = ":srcs",
    out_binaries = [
        "osirrox",
        "xorrecord",
        "xorriso",
        "xorriso-dd-target",
        "xorriso-tcltk",
        "xorrisofs",
    ],
    targets = [
        "install",
    ],
)

genrule(
    name = "xorriso",
    srcs = [":bin"],
    outs = ["bin/xorriso"],
    cmd = "cp `ls $(locations //:bin) | grep /bin/xorriso$$` $(@)",
    executable = True,
)
