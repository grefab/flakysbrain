load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
    name = "proto",
    srcs = ["api.proto"],
    visibility = ["//visibility:public"],
)

cc_proto_library(
    name = "cc_proto",
    visibility = ["//visibility:public"],
    deps = [":proto"],
)

cc_grpc_library(
    name = "cc_grpc_proto",
    srcs = [":proto"],
    grpc_only = True,
    proto_only = False,
    visibility = ["//visibility:public"],
    well_known_protos = "@com_google_protobuf//:well_known_protos",
    deps = [
        ":cc_proto",
        "@com_github_grpc_grpc//:grpc++",
    ],
)

cc_library(
    name = "service",
    srcs = glob(
        [
            "**/*.cpp",
            "**/*.h",
        ],
    ),
    visibility = ["//visibility:public"],
    deps = [
        ":cc_grpc_proto",
        "//brain",
    ],
)
