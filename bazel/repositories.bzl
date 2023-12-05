load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def data_dependency():
    http_archive(
        name = "limine",
        build_file = "//:third_party/limine/limine.BUILD",
        sha256 = "a70f9b7e7788474bc5c55fc57a46afa3ef072654b76a81495c696192b902eb04",
        strip_prefix = "limine-5.20231124.0-binary",
        urls = [
            "https://github.com/limine-bootloader/limine/archive/refs/tags/v5.20231124.0-binary.tar.gz",
        ],
    )
