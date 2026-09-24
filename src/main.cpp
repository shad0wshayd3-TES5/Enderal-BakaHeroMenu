#include "Events.h"
#include "Hooks.h"
#include "Manager.h"

namespace
{
	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case SKSE::MessagingInterface::kPostLoad:
			Hooks::Install();
			break;
		case SKSE::MessagingInterface::kPostLoadGame:
			Manager::GetSingleton()->LoadSave();
			break;
		case SKSE::MessagingInterface::kDataLoaded:
			Events::Install();
			Manager::GetSingleton()->LoadForms();
			break;
		default:
			break;
		}
	}
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);
	SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);
	return true;
}
