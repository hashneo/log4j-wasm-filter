// NOLINT(namespace-envoy)
#pragma once

#include <string>
#include <unordered_map>
#include <regex>

#include "proxy_wasm_intrinsics.h"

#include "root_context.h"

class AddHeaderContext : public Context {
public:
    explicit AddHeaderContext(uint32_t id, RootContext *root) : Context(id, root),
                                                                root_(static_cast<AddHeaderRootContext *>(static_cast<void *>(root))) {}

    void onCreate() override;

    FilterHeadersStatus onRequestHeaders(uint32_t headers, bool end_of_stream) override;
    FilterDataStatus onRequestBody(size_t body_buffer_length, bool end_of_stream) override;
    FilterHeadersStatus onResponseHeaders(uint32_t headers, bool end_of_stream) override;

    void onDone() override;
    void onLog() override;
    void onDelete() override;

private:

    AddHeaderRootContext *root_;

    bool hackFound = false;
};
