load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "glfw_opengl3",
    srcs = [
        "imgui_impl_glfw.cpp",
        "imgui_impl_opengl3.cpp",
    ],
    hdrs = [
        "imgui_impl_glfw.h",
        "imgui_impl_opengl3.h",
        "imgui_impl_opengl3_loader.h",
    ],
    linkopts = [
        "-framework OpenGL",
    ],
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        "//3rdparty/glfw",
        "//3rdparty/imgui-docking",
    ],
)
