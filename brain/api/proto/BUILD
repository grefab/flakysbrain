load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
    name = "api_proto",
    srcs = ["api.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "@com_google_protobuf//:empty_proto",
    ],
)

cc_proto_library(
    name = "api_cc_proto",
    visibility = ["//visibility:public"],
    deps = [":api_proto"],
)

cc_grpc_library(
    name = "api_grpc_proto",
    srcs = [":api_proto"],
    grpc_only = True,
    proto_only = False,
    visibility = ["//visibility:public"],
    well_known_protos = "@com_google_protobuf//:well_known_protos",
    deps = [
        ":api_cc_proto",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
