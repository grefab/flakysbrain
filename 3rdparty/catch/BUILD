load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "catch",
    srcs = ["catch.hpp"],
    defines = ["CATCH_CONFIG_ENABLE_BENCHMARKING"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "main",
    srcs = ["main_test.cpp"],
    linkstatic = True,
    visibility = ["//visibility:public"],
    deps = [":catch"],
    alwayslink = True,
)
