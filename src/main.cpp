//#include "Animation/Animation.h"
#include <Events/Events.h>
//#include <Events/EventSinks/CrosshairRefMgr.h>
//#include "Hooks.h"
//#include "Input/Input.h"
//#include "Loot.h"
//#include "Scaleform/Scaleform.h"
//#include "LOTD/LOTD.h"
//#include "Items/GFxItem.h"

namespace
{
	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kDataLoaded:
#ifndef NDEBUG
			InputHandler::Register();
#endif

			//Animation::AnimationManager::Install();

			Events::Register();
			//Scaleform::Register();

			Settings::LoadSettings();
			//LOTD::LoadLists();
			break;
		case SKSE::MessagingInterface::kPostPostLoad:
		{
			//Completionist_Integration::RegisterListener();
		}
		break;
		}
	}

	void InitializeLog(const SKSE::PluginDeclaration* plugin)
	{
#ifndef NDEBUG
		auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
		auto path = SKSE::log::log_directory();
		if (!path) {
			stl::report_and_fail("Failed to find standard logging directory"sv);
		}

		*path /= fmt::format("{}.log", plugin->GetName());
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

#ifndef NDEBUG
		const auto level = spdlog::level::trace;
#else
		const auto level = spdlog::level::info;
#endif

		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
		log->set_level(level);
		log->flush_on(level);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);
	}
}

/*extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v;

	v.PluginVersion(Plugin::VERSION);
	v.PluginName(Plugin::NAME);

	v.UsesAddressLibrary(true);
	//v.CompatibleVersions({ SKSE::RUNTIME_LATEST });

	return v;
}();

EXTERN_C [[maybe_unused]] __declspec(dllexport) bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}*/

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
	auto* plugin = SKSE::PluginDeclaration::GetSingleton();
	auto version = plugin->GetVersion();

	//InitializeLog(plugin);
	SKSE::log::info("{} {}.b is loading...", plugin->GetName(), version);
	SKSE::Init(skse);
//	SKSE::AllocTrampoline(1 << 6);
	auto message = SKSE::GetMessagingInterface();
	if (!message->RegisterListener(MessageHandler)) {
		return false;
	}

	//Hooks::Install();

	return true;
}
