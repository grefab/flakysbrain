package(default_visibility = ["//visibility:public"])

proto_library(
    name = "proto",
    srcs = ["api.proto"],
)

load("@build_stack_rules_proto//cpp:cpp_grpc_library.bzl", "cpp_grpc_library")

cpp_grpc_library(
    name = "cpp_proto",
    deps = [":proto"],
)

cc_library(
    name = "service",
    srcs = glob(
        [
            "**/*.cpp",
            "**/*.h",
        ],
    ),
    deps = [
        ":cpp_proto",
    ],
)
