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
