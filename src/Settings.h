#pragma once

namespace Settings
{
	struct SettingsImpl
	{
		bool ApplySeducerBonus{ false };
		bool ApplyMesmerizeBonus{ false };
		bool IgnoreLearningGold{ false };
		bool IgnoreLearningPoints{ false };
		bool IgnoreCraftingGold{ false };
		bool IgnoreCraftingPoints{ false };
		bool SkipCallback{ false };
	};

	static SettingsImpl* GetSingleton()
	{
		static SettingsImpl impl;
		return std::addressof(impl);
	}
}
