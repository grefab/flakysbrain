load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "imnodes",
    srcs = [
        "imnodes.cpp",
        "imnodes.h",
        "imnodes_internal.h",
    ],
    hdrs = [
        "imnodes.h",
    ],
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = ["//3rdparty/imgui-docking"],
)
