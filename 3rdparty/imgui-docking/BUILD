load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "imgui-docking",
    srcs = [
        "imgui.cpp",
        "imgui_demo.cpp",
        "imgui_draw.cpp",
        "imgui_internal.h",
        "imgui_tables.cpp",
        "imgui_widgets.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "misc/cpp/imgui_stdlib.cpp",
        "misc/cpp/imgui_stdlib.h",
    ],
    hdrs = [
        "imconfig.h",
        "imgui.h",
        "imgui_internal.h",
        "imstb_textedit.h",
        "misc/cpp/imgui_stdlib.h",
    ],
    defines = ["IMGUI_USE_WCHAR32"],
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
)
