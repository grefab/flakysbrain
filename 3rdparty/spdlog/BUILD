load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "spdlog",
    srcs = glob(["src/*.cpp"]),
    hdrs = glob(["include/**/*.h"]),
    defines = [
        "SPDLOG_COMPILED_LIB",
        "SPDLOG_FMT_EXTERNAL",
    ],
    includes = ["include"],
    linkopts = ["-lpthread"],
    visibility = ["//visibility:public"],
    deps = ["//3rdparty/fmt"],
)
