package(default_visibility = ["//visibility:public"])

exports_files(["**/*"])

filegroup(
    name = "all",
    srcs = glob(["**/*"]),
)

cc_library(
    name = "limine",
    hdrs = [
        "limine.h",
    ],
)

cc_binary(
    name = "limine_deploy",
    srcs = [
        "limine.c",
        "limine.h",
        "limine-bios-hdd.h",
    ],
)
