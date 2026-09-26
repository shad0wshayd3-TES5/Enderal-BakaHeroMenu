#pragma once

#include "Manager.h"
#include "MCM.h"

namespace Events
{
	class MenuOpenCloseEvent :
		public REX::TSingleton<MenuOpenCloseEvent>,
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{
	public:
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
		{
			if (a_event && a_event->opening && a_event->menuName == "CustomMenu"sv)
			{
				if (auto ui = RE::UI::GetSingleton())
					if (auto view = ui->GetMovieView("CustomMenu"sv);
						view &&
						view->GetMovieDef() &&
						REX::STR::ICONTAINS(view->GetMovieDef()->GetFileURL(), "00E_HeroMenu.swf"sv))
					{
						Manager::GetSingleton()->UpdateCache();
						Manager::GetSingleton()->UpdateMenuName(view);
					}
			}

			if (a_event && !a_event->opening && a_event->menuName == "Journal Menu"sv)
			{
				MCM::GetSingleton()->LoadSettings();
			}

			return RE::BSEventNotifyControl::kContinue;
		}
	};

	static void Install()
	{
		if (auto ui = RE::UI::GetSingleton())
			ui->AddEventSink<RE::MenuOpenCloseEvent>(MenuOpenCloseEvent::GetSingleton());
	}
}
