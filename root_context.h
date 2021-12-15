#pragma once

#include <string>
#include <regex>

#include "proxy_wasm_intrinsics.h"

class AddHeaderRootContext : public RootContext {
public:
    explicit AddHeaderRootContext(uint32_t id, std::string_view root_id) : RootContext(id, root_id) {
    }

    bool onConfigure(size_t /* configuration_size */) override;
    bool onStart(size_t) override;

    std::string regex_value_;
    std::regex regex_;
};
