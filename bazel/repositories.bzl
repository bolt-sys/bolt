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

    http_archive(
        name = "xorriso",
        build_file = "//:third_party/xorriso/xorriso.BUILD",
        sha256 = "786f9f5df9865cc5b0c1fecee3d2c0f5e04cab8c9a859bd1c9c7ccd4964fdae1",
        strip_prefix = "xorriso-1.5.6",
        urls = [
            "https://www.gnu.org/software/xorriso/xorriso-1.5.6.pl02.tar.gz",
        ],
    )
