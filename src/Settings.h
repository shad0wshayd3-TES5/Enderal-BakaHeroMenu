#pragma once

namespace Settings
{
	struct SettingsImpl
	{
		bool ApplySeducerBonus{ false };
		bool IgnoreGold{ false };
		bool IgnorePoints{ false };
		bool SkipCallback{ false };
	};

	static SettingsImpl* GetSingleton()
	{
		static SettingsImpl impl;
		return std::addressof(impl);
	}
}
