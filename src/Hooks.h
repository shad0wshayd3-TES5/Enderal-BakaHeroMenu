#pragma once

#include "Manager.h"

namespace Hooks
{
	class hkProcessEvent
	{
	private:
		static constexpr std::array<const char*, 18> SkillNames{
			"einhand",           // OneHanded
			"zweihand",          // TwoHanded
			"bogenkunst",        // Archery
			"parade",            // Block
			"schmieden",         // Smithing
			"schwereruestung",   // HeavyArmor
			"leichteruestung",   // LightArmor
			"taschendiebstahl",  // Pickpocket
			"schlossknacken",    // Lockpicking
			"schleichen",        // Sneak
			"alchimie",          // Alchemy
			"rethorik",          // Speech
			"mentalismus",       // Alteration
			"entropie",          // Conjuration
			"elementarismus",    // Destruction
			"psionik",           // Illusion
			"lichtmagie",        // Restoration
			"verzauberung",      // Enchanting
		};

	private:
		static std::optional<std::int32_t> GetSelectedSkill(RE::GFxValue& a_mc)
		{
			for (std::int32_t i = 0; i < 18; i++)
			{
				RE::GFxValue member;
				if (a_mc.GetMember(SkillNames[i], &member))
				{
					RE::GFxValue visible;
					if (auto success = member.GetMember("_visible", &visible);
						success && visible.IsBool() && visible.GetBool())
					{
						return i;
					}
				}
			}

			return std::nullopt;
		};

		static RE::BSEventNotifyControl ProcessEvent(RE::MenuControls* a_this, RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_source)
		{
			if (a_event && *a_event)
			{
				if (auto ui = RE::UI::GetSingleton();
					ui && ui->IsMenuOpen("CustomMenu"sv) && !ui->IsMenuOpen("Console"sv))
				{
					RE::GFxValue movieclip;
					if (auto view = ui->GetMovieView("CustomMenu"sv);
						view &&
						view->GetMovieDef() &&
						REX::STR::ICONTAINS(view->GetMovieDef()->GetFileURL(), "00E_HeroMenu.swf"sv) &&
						view->GetVariable(&movieclip, "heromenu_mc.description"))
					{
						if (auto buttonEvent = (*a_event)->AsButtonEvent();
							buttonEvent && buttonEvent->IsDown())
						{
							if (Manager::GetSingleton()->QOverrideMessage())
								return _ProcessEvent(a_this, a_event, a_source);

							switch (buttonEvent->GetIDCode())
							{
							case 0:  // Click
								if (auto idx = GetSelectedSkill(movieclip))
									Manager::GetSingleton()->TryIncreaseSkill(*idx);
								break;
							case 15:  // Cancel
							case 35:  // Quick Stats
								if (auto queue = RE::UIMessageQueue::GetSingleton())
									queue->AddMessage("CustomMenu", RE::UI_MESSAGE_TYPE::kHide, nullptr);
								break;
							default:
								break;
							}
						}
					}
				}
			}

			return _ProcessEvent(a_this, a_event, a_source);
		}

		inline static REL::THookVFT _ProcessEvent{ REL::EHookStep::None, RE::VTABLE_MenuControls[0], 0x01, ProcessEvent };

	public:
		static void Install()
		{
			_ProcessEvent.Enable();
		}
	};

	static void Install()
	{
		hkProcessEvent::Install();
	}
}
