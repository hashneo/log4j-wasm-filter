// NOLINT(namespace-envoy)
#include "filter.h"

static RegisterContextFactory register_AddHeaderContext(CONTEXT_FACTORY(AddHeaderContext),
                                                        ROOT_FACTORY(AddHeaderRootContext),
                                                        "log4j_cve_filter");

void AddHeaderContext::onCreate() {
    LOG_DEBUG(std::string("onCreate " + std::to_string(id())));
}

FilterHeadersStatus AddHeaderContext::onRequestHeaders(uint32_t, bool) {
    LOG_DEBUG(std::string("onRequestHeaders ") + std::to_string(id()));

    auto result = getRequestHeaderPairs();
    auto pairs = result->pairs();

    LOG_INFO(std::string("headers: ") + std::to_string(pairs.size()));

    for (auto& p : pairs) {
        std::string v = std::string(p.second);

        std::cmatch m;

        if(std::regex_search(v.c_str(), m, root_->regex_) ) {
            LOG_INFO( std::string("hack found in header ") +  std::string(p.first) + std::string(" -> ") + v);
            sendLocalResponse(403, "Forbidden", "Potential Remote Command Execution: Log4j CVE-2021-44228" , {});
            return FilterHeadersStatus::StopIteration;
        }

        LOG_INFO(std::string(p.first) + std::string(" -> ") + v);
    }

    return FilterHeadersStatus::Continue;
}

FilterHeadersStatus AddHeaderContext::onResponseHeaders(uint32_t, bool) {
    LOG_DEBUG(std::string("onResponseHeaders ") + std::to_string(id()));
    return FilterHeadersStatus::Continue;
}

FilterDataStatus AddHeaderContext::onRequestBody(size_t body_buffer_length, bool end_of_stream) {

    auto buffer = getBufferBytes(WasmBufferType::HttpRequestBody, 0, body_buffer_length);
    auto body = std::string(buffer->view());

    std::cmatch m;

    if(std::regex_search(body.c_str(), m, root_->regex_) ) {
        LOG_DEBUG("hack found in the payload and the connection will be terminated");
        sendLocalResponse(403, "Forbidden", "Potential Remote Command Execution: Log4j CVE-2021-44228" , {});
        return FilterDataStatus::StopIterationNoBuffer;
    }

    return FilterDataStatus::Continue;
}

void AddHeaderContext::onDone() {
    LOG_DEBUG(std::string("onDone " + std::to_string(id())));
}

void AddHeaderContext::onLog() {
    LOG_DEBUG(std::string("onLog " + std::to_string(id())));
}

void AddHeaderContext::onDelete() {
    LOG_DEBUG(std::string("onDelete " + std::to_string(id())));
}
