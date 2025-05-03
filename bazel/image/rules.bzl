"""
Bazel rules for creating disk images
"""

load("impl/iso_image.bzl", _iso_image = "iso_image")

iso_image = _iso_image
