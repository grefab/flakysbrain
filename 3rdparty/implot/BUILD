load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "implot",
    srcs = [
        "implot.cpp",
        "implot.h",
        "implot_demo.cpp",
        "implot_internal.h",
        "implot_items.cpp",
    ],
    hdrs = [
        "implot.h",
        "implot_internal.h",
    ],
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = ["//3rdparty/imgui-docking"],
)
