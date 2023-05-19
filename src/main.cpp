//#include <Hooks/Hooks.h>

namespace {
    void MessageHandler(SKSE::MessagingInterface::Message *a_msg) {
        switch (a_msg->type) {
            case SKSE::MessagingInterface::kDataLoaded:
                //Events::Register();
                // register hooks/events/etc.
                break;
        }
    }

    void InitializeLog(const SKSE::PluginDeclaration *plugin) {
#ifndef NDEBUG
        const auto level = spdlog::level::trace;
		auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
        const auto level = spdlog::level::info;
        auto path = SKSE::log::log_directory();
        if (!path) {
            stl::report_and_fail("Failed to find standard logging directory"sv);
        }

        *path /= fmt::format("{}.log", plugin->GetName());
        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif
        auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
        log->set_level(level);
        log->flush_on(level);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    auto *plugin = SKSE::PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();

    InitializeLog(plugin);
    SKSE::log::info("{} {}.b is loading...", plugin->GetName(), version);
    SKSE::Init(skse);
    if (!SKSE::GetMessagingInterface()->RegisterListener(MessageHandler)) {
        return false;
    }

    //RE::PlayerControls::jumpHandler
    return true;
}

