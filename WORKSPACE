workspace(name = "isotronic")

# These rules are built-into Bazel but we need to load them first to download more rules

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

#
# gRPC (includes protobuf)
#
git_repository(
    name = "com_github_grpc_grpc",
    #    tag = "v1.47.0",  # should correspond to commit & shallow_since
    commit = "2717aa8c9b768de99c811628af4e8c30d6c2da4f",
    remote = "https://github.com/grpc/grpc.git",
    shallow_since = "1655843987 -0700",
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")

grpc_extra_deps()

#
# boost
#

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "fb9f3c9a6011f966200027843d894923ebc9cd0b",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1626494016 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

git_repository(
    name = "com_github_ericniebler_range-v3",
    commit = "0fa54d7de5dc00655e38a08e87cda61f7aa6d5b9",
    remote = "https://github.com/ericniebler/range-v3.git",
    shallow_since = "1654131509 -0500",
)

#
# SDL
#
new_local_repository(
    name = "sdl2",
    build_file = "3rdparty/sdl.build",
    path = "/Library/Frameworks/SDL2.framework",
)

#
# CMAKE / foreign rules
#
# Group the sources of the library so that CMake rule have access to it
all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

http_archive(
    name = "rules_foreign_cc",
    sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
    strip_prefix = "rules_foreign_cc-0.9.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/refs/tags/0.9.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://bazelbuild.github.io/rules_foreign_cc/0.8.0/flatten.html#rules_foreign_cc_dependencies
rules_foreign_cc_dependencies()

#
# OpenCV
#

http_archive(
    name = "opencv",
    build_file_content = all_content,
    sha256 = "a5839ba52aaced93f1b70d416bbdde1e32bcfcafb38cc9c6bc73331c09cb1a5c",
    strip_prefix = "opencv-778faddbd8e997af57202cf4fe633d54f538fe93",
    urls = [
        "https://github.com/opencv/opencv/archive/778faddbd8e997af57202cf4fe633d54f538fe93.zip",
    ],
)
