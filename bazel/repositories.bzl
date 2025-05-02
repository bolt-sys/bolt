"""
Bazel repositories for third-party dependencies.
"""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def source_dependencies():
    http_archive(
        name = "limine",
        build_file = "//:third_party/limine/limine.BUILD",
        sha256 = "b6bef3c3b1d22f2ed9642b1d9ad89992065a1fe43e82f81968cd55f364353704",
        strip_prefix = "limine-9.2.3-binary",
        urls = [
            "https://github.com/limine-bootloader/limine/archive/refs/tags/v9.2.3-binary.tar.gz",
        ],
    )

    http_archive(
        name = "xorriso",
        build_file = "//:third_party/xorriso/xorriso.BUILD",
        sha256 = "584d2af802a4255d7f25438b572a1ffbab93f4121f70414ca1e8b0111f313e1f",
        strip_prefix = "xorriso-1.5.7",
        urls = [
            "https://www.gnu.org/software/xorriso/xorriso-1.5.7.tar.gz",
        ],
    )
