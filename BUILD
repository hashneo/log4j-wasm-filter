load("@proxy_wasm_cpp_sdk//bazel/wasm:wasm.bzl", "wasm_cc_binary")

licenses(["notice"])  # Apache 2

wasm_cc_binary(
    name = "filter.wasm",
    srcs = ["filter.h", "filter.cc", "root_context.h", "root_context.cc"],
    deps = [
        "@proxy_wasm_cpp_sdk//:proxy_wasm_intrinsics",
    ],
)
