#include "root_context.h"

bool AddHeaderRootContext::onConfigure(size_t config_buffer_length) {
    auto conf = getBufferBytes(WasmBufferType::PluginConfiguration, 0, config_buffer_length);
    regex_value_ = conf->toString();
    LOG_DEBUG("onConfigure regex loaded" + regex_value_);

    try {
        regex_ = std::regex(regex_value_, std::regex_constants::ECMAScript);
    }
    catch(const std::regex_error& e){
        LOG_ERROR(e.what());
    }
    catch(...){
        LOG_ERROR("onConfigure general load exception caught" );
    }

    return true;
}

bool AddHeaderRootContext::onStart(size_t) {
    LOG_DEBUG("onStart");
    return true;
}
