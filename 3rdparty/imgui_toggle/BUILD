load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "imgui_toggle",
    srcs = [
        "imgui_toggle.cpp",
        "imgui_toggle.h",
    ],
    hdrs = [
        "imgui_toggle.h",
    ],
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = ["//3rdparty/imgui-docking"],
)
