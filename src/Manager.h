#pragma once

#include "Settings.h"

class Manager :
	public REX::TSingleton<Manager>
{
public:
	enum Book : std::int32_t
	{
		kApprentice,
		kAdept,
		kExpert,
		kMaster,
		kBookTotal,
	};

	enum Stat : std::int32_t
	{
		kStat,
		kBase,
		kStatTotal,
	};

	enum Globals : std::int32_t
	{
		kExpMult,
		kExpMultSlope,
		kPlayerXP,
		kPlayerNeededXP,
		kPlayerLevel,
		kCraftingPoints,
		kLearningPoints,
		kTalentPoints,
		kGlobTotal,
	};

	void LoadForms()
	{
		UpdateForms();
		UpdateBookValues();
	}

	void LoadSave()
	{
		UpdateClassName();
		// UpdatePlayerName();
	}

	void Update()
	{
		UpdateClassName();
		UpdateAttributes();
		UpdateStatValues();
		UpdatePlayerName();
		UpdatePlayerGold();
	}

public:
	bool QOverrideMessage() const
	{
		return overrideMessage;
	}

	void ResetOverrideMessage()
	{
		overrideMessage = false;
	}

private:
	inline std::int32_t GetAttrIndex(RE::ActorValue a_actorValue) const
	{
		switch (a_actorValue)
		{
		case RE::ActorValue::kLastFlattered:
			return 3;
		default:
			return std::to_underlying(a_actorValue) - std::to_underlying(RE::ActorValue::kHealth);
		}
	}

	inline std::int32_t GetStatIndex(RE::ActorValue a_actorValue) const
	{
		return std::to_underlying(a_actorValue) - std::to_underlying(RE::ActorValue::kOneHanded);
	}

private:
	float GetGlobalValue(Globals a_value)
	{
		switch (a_value)
		{
		case Globals::kExpMult:
			return EXPMult ? EXPMult->value : 0.0f;
		case Globals::kExpMultSlope:
			return EXPMultSlope ? EXPMultSlope->value : 0.0f;
		case Globals::kPlayerXP:
			return PlayerXP ? PlayerXP->value : 0.0f;
		case Globals::kPlayerNeededXP:
			return PlayerNeededXP ? PlayerNeededXP->value : 0.0f;
		case Globals::kPlayerLevel:
			return PlayerLevel ? PlayerLevel->value : 0.0f;
		case Globals::kCraftingPoints:
			return CraftingPoints ? CraftingPoints->value : 0.0f;
		case Globals::kLearningPoints:
			return LearningPoints ? LearningPoints->value : 0.0f;
		case Globals::kTalentPoints:
			return TalentPoints ? TalentPoints->value : 0.0f;
		default:
			return 0;
		}
	}

private:
	float GetAttrValue(RE::ActorValue a_actorValue)
	{
		auto index = GetAttrIndex(a_actorValue);
		return attrValue[index][kStat];
	}

	float GetAttrBaseValue(RE::ActorValue a_actorValue)
	{
		auto index = GetAttrIndex(a_actorValue);
		return attrValue[index][kBase];
	}

	float GetAttrMod(RE::ActorValue a_actorValue)
	{
		auto index = GetAttrIndex(a_actorValue);
		return attrValue[index][kStat] - attrValue[index][kBase];
	}

	void UpdateAttribute(RE::ActorValue a_actorValue)
	{
		if (auto player = RE::PlayerCharacter::GetSingleton())
		{
			auto index = GetAttrIndex(a_actorValue);
			attrValue[index][kStat] = player->GetActorValue(a_actorValue);
			attrValue[index][kBase] = player->GetBaseActorValue(a_actorValue);
		}
	}

	void UpdateAttributes()
	{
		UpdateAttribute(RE::ActorValue::kHealth);
		UpdateAttribute(RE::ActorValue::kMagicka);
		UpdateAttribute(RE::ActorValue::kStamina);
		UpdateAttribute(RE::ActorValue::kLastFlattered);
	}

private:
	float GetStatValue(RE::ActorValue a_actorValue)
	{
		auto index = GetStatIndex(a_actorValue);
		return statValue[index][kStat];
	}

	float GetStatBaseValue(RE::ActorValue a_actorValue)
	{
		auto index = GetStatIndex(a_actorValue);
		return statValue[index][kBase];
	}

	float GetStatMod(RE::ActorValue a_actorValue)
	{
		auto index = GetStatIndex(a_actorValue);
		return statValue[index][kStat] - statValue[index][kBase];
	}

	void UpdateStatValue(RE::ActorValue a_actorValue)
	{
		if (auto player = RE::PlayerCharacter::GetSingleton())
		{
			auto index = GetStatIndex(a_actorValue);
			statValue[index][kStat] = player->GetActorValue(a_actorValue);
			statValue[index][kBase] = player->GetBaseActorValue(a_actorValue);
		}
	}

	void UpdateStatValues()
	{
		UpdateStatValue(RE::ActorValue::kOneHanded);
		UpdateStatValue(RE::ActorValue::kTwoHanded);
		UpdateStatValue(RE::ActorValue::kArchery);
		UpdateStatValue(RE::ActorValue::kBlock);
		UpdateStatValue(RE::ActorValue::kSmithing);
		UpdateStatValue(RE::ActorValue::kHeavyArmor);
		UpdateStatValue(RE::ActorValue::kLightArmor);
		UpdateStatValue(RE::ActorValue::kPickpocket);
		UpdateStatValue(RE::ActorValue::kLockpicking);
		UpdateStatValue(RE::ActorValue::kSneak);
		UpdateStatValue(RE::ActorValue::kAlchemy);
		UpdateStatValue(RE::ActorValue::kSpeech);
		UpdateStatValue(RE::ActorValue::kAlteration);
		UpdateStatValue(RE::ActorValue::kConjuration);
		UpdateStatValue(RE::ActorValue::kDestruction);
		UpdateStatValue(RE::ActorValue::kIllusion);
		UpdateStatValue(RE::ActorValue::kRestoration);
		UpdateStatValue(RE::ActorValue::kEnchanting);
	}

private:
	void UpdatePlayerGold()
	{
		if (auto player = RE::PlayerCharacter::GetSingleton();
			player && Gold001)
		{
			playerGold = player->GetItemCount(Gold001);
		}
	}

	void UpdatePlayerName()
	{
		if (auto player = RE::PlayerCharacter::GetSingleton())
			playerName = player->GetDisplayFullName();
	}

private:
	void SetClassName(std::string_view a_name)
	{
		playerClass = a_name;
	}

	void UpdateClassName()
	{
		class UpdateClassNameCallback :
			public RE::BSScript::IStackCallbackFunctor
		{
		public:
			virtual void operator()(RE::BSScript::Variable a_result) override
			{
				if (a_result.IsString())
					Manager::GetSingleton()->SetClassName(a_result.GetString());
			}

			virtual bool CanSave() const override { return false; }
			virtual void SetObject(const RE::BSTSmartPointer<RE::BSScript::Object>&) override { return; }
		};

		RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback{ new UpdateClassNameCallback };
		if (auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton())
			vm->DispatchStaticCall("EnderalFunctions", "GetPlayerClassNameGlobal", RE::MakeFunctionArguments(), callback);
	}

private:
	std::int32_t CalculateGoldCost(std::int32_t a_base)
	{
		auto mod{ 1.0f };
		if (auto player = RE::PlayerCharacter::GetSingleton())
		{
			auto min{ 1.3f }, max{ 3.3f }, bmi{ 1.05f }, bml{ 1.0f };
			if (auto gmst = RE::GameSettingCollection::GetSingleton())
			{
				if (auto setting = gmst->GetSetting("fBarterMin"))
					min = setting->GetFloat();
				if (auto setting = gmst->GetSetting("fBarterMax"))
					max = setting->GetFloat();
				if (auto setting = gmst->GetSetting("fBarterBuyMin"))
					bmi = setting->GetFloat();
				if (auto setting = gmst->GetSetting("fMinBuyMult"))
					bml = setting->GetFloat();
			}

			mod = std::max(bmi, max - (max - min) * (std::min(GetStatValue(RE::ActorValue::kSpeech), 100.0f) / 100.0f));
			RE::BGSEntryPoint::HandleEntryPoint(
				RE::BGSEntryPoint::ENTRY_POINT::kModBuyPrices,
				player,
				nullptr,
				&mod);

			if (Settings::GetSingleton()->ApplySeducerBonus)
			{
				if (Seducer && player->HasPerk(Seducer))
					mod *= 0.90f;
			}

			if (Settings::GetSingleton()->ApplyMesmerizeBonus)
			{
				if (Mesmerize03 && player->HasSpell(Mesmerize03))
					mod *= 1.0f + ((GetStatValue(RE::ActorValue::kIllusion) / 6.0f) + 14.0f) / 100.0f;
				else if (Mesmerize02 && player->HasSpell(Mesmerize02))
					mod *= 1.0f + ((GetStatValue(RE::ActorValue::kIllusion) / 6.0f) + 10.0f) / 100.0f;
				else if (Mesmerize01 && player->HasSpell(Mesmerize01))
					mod *= 1.0f + ((GetStatValue(RE::ActorValue::kIllusion) / 6.0f) + 07.0f) / 100.0f;
			}

			mod = std::max(std::max(mod, bml), 1.0f);
		}

		return static_cast<std::int32_t>(roundf(a_base * mod));
	}

	std::int32_t GetGoldCost(RE::ActorValue a_actorValue, Book a_book)
	{
		auto index = GetStatIndex(a_actorValue);
		return CalculateGoldCost(bookValue[index][a_book]);
	}

	std::int32_t GetGoldCost(RE::ActorValue a_actorValue)
	{
		auto value{ 0.0f };
		if (auto player = RE::PlayerCharacter::GetSingleton())
			value = player->GetBaseActorValue(a_actorValue);

		if (value < 25)
			return GetGoldCost(a_actorValue, Book::kApprentice);
		else if (value < 50)
			return GetGoldCost(a_actorValue, Book::kAdept);
		else if (value < 75)
			return GetGoldCost(a_actorValue, Book::kExpert);
		else
			return GetGoldCost(a_actorValue, Book::kMaster);
	}

private:
	void ModGold(std::int32_t a_cost)
	{
		if (auto player = RE::PlayerCharacter::GetSingleton();
			player && Gold001)
		{
			player->RemoveItem(Gold001, a_cost, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
		}
		UpdatePlayerGold();
	}

	void ModGlobal(RE::TESGlobal* a_global, std::int32_t a_mod)
	{
		if (a_global)
			a_global->value -= a_mod;
	}

	void ModValue(RE::ActorValue a_actorValue, std::int32_t a_mod)
	{
		if (auto player = RE::PlayerCharacter::GetSingleton())
			player->ModBaseActorValue(a_actorValue, static_cast<float>(a_mod));
		UpdateStatValue(a_actorValue);
	}

private:
	void IncreaseLearningSkill(RE::ActorValue a_actorValue, std::int32_t a_cost)
	{
		if (!Settings::GetSingleton()->IgnoreLearningGold)
			ModGold(a_cost);
		if (!Settings::GetSingleton()->IgnoreLearningPoints)
			ModGlobal(LearningPoints, 1);
		ModValue(a_actorValue, 1);
		UpdateMenuData();
	}

	void IncreaseCraftingSkill(RE::ActorValue a_actorValue, std::int32_t a_cost)
	{
		if (!Settings::GetSingleton()->IgnoreCraftingGold)
			ModGold(a_cost);
		if (!Settings::GetSingleton()->IgnoreCraftingPoints)
			ModGlobal(CraftingPoints, 1);
		ModValue(a_actorValue, 1);
		UpdateMenuData();
	}

public:
	void IncreaseSkill(RE::ActorValue a_actorValue, std::int32_t a_cost)
	{
		switch (a_actorValue)
		{
		case RE::ActorValue::kOneHanded:
		case RE::ActorValue::kTwoHanded:
		case RE::ActorValue::kArchery:
		case RE::ActorValue::kBlock:
		case RE::ActorValue::kHeavyArmor:
		case RE::ActorValue::kLightArmor:
		case RE::ActorValue::kSneak:
		case RE::ActorValue::kAlteration:
		case RE::ActorValue::kConjuration:
		case RE::ActorValue::kDestruction:
		case RE::ActorValue::kIllusion:
		case RE::ActorValue::kRestoration:
			IncreaseLearningSkill(a_actorValue, a_cost);
			break;
		case RE::ActorValue::kSmithing:
		case RE::ActorValue::kPickpocket:
		case RE::ActorValue::kLockpicking:
		case RE::ActorValue::kAlchemy:
		case RE::ActorValue::kSpeech:
		case RE::ActorValue::kEnchanting:
			IncreaseCraftingSkill(a_actorValue, a_cost);
			break;
		}
	}

private:
	class HeroMessageBoxCallback :
		public RE::IMessageBoxCallback
	{
	public:
		inline static RE::BSTArray<RE::BSString> Buttons{ "$Yes", "$Cancel" };

	public:
		HeroMessageBoxCallback() = delete;
		HeroMessageBoxCallback(RE::ActorValue a_actorValue, std::int32_t a_cost) :
			m_actorValue(a_actorValue),
			m_cost(a_cost)
		{}

		virtual void Run(std::uint8_t a_button) override
		{
			if (a_button == 0)
				Manager::GetSingleton()->IncreaseSkill(m_actorValue, m_cost);
			Manager::GetSingleton()->ResetOverrideMessage();
		}

	private:
		RE::ActorValue m_actorValue{};
		std::int32_t   m_cost{};
	};

	void RequestCallback(RE::ActorValue a_actorValue, std::int32_t a_cost, bool a_learning)
	{
		// clang-format off
		auto text = std::format(
			"Do you want to increase {}?\n\nPrice: {}p, 1 {} Point\n\nCurrent Pennies: {}\n Current {} Points: {}"sv,
			a_actorValue,
			a_cost,
			(a_learning ? "Learning"sv : "Crafting"sv),
			playerGold,
			(a_learning ? "Learning"sv : "Crafting"sv),
			(a_learning ? GetGlobalValue(kLearningPoints) : GetGlobalValue(kCraftingPoints)));

		RE::BSString message{ text };
		RE::BSTSmartPointer<RE::IMessageBoxCallback> call{ new HeroMessageBoxCallback(a_actorValue, a_cost) };
		RE::MessageBoxMenu::Create(message, call, 0, 25, 10, HeroMessageBoxCallback::Buttons);
		// clang-format on

		overrideMessage = true;
	}

	void TryIncreaseLearningSkill(RE::ActorValue a_actorValue)
	{
		auto cost = GetGoldCost(a_actorValue);
		if (!Settings::GetSingleton()->IgnoreLearningGold &&
			cost > playerGold)
		{
			auto message = std::format(
				"You do not have enough pennies to increase {}!\nPrice: {}p"sv,
				a_actorValue, cost);
			return RE::DebugMessageBox(message.c_str());
		}

		if (!Settings::GetSingleton()->IgnoreLearningPoints &&
			!GetGlobalValue(kLearningPoints))
		{
			auto message = std::format(
				"You do not have enough Learning Points to increase {}!"sv,
				a_actorValue);
			return RE::DebugMessageBox(message.c_str());
		}

		if (!Settings::GetSingleton()->SkipCallback)
			return RequestCallback(a_actorValue, cost, true);
		IncreaseLearningSkill(a_actorValue, cost);
	}

	void TryIncreaseCraftingSkill(RE::ActorValue a_actorValue)
	{
		auto cost = GetGoldCost(a_actorValue);
		if (!Settings::GetSingleton()->IgnoreCraftingGold &&
			cost > playerGold)
		{
			auto message = std::format(
				"You do not have enough pennies to increase {}!\nPrice: {}p"sv,
				a_actorValue, cost);
			return RE::DebugMessageBox(message.c_str());
		}

		if (!Settings::GetSingleton()->IgnoreCraftingPoints &&
			!GetGlobalValue(kCraftingPoints))
		{
			auto message = std::format(
				"You do not have enough Crafting Points to increase {}!"sv,
				a_actorValue);
			return RE::DebugMessageBox(message.c_str());
		}

		if (!Settings::GetSingleton()->SkipCallback)
			return RequestCallback(a_actorValue, cost, false);
		IncreaseCraftingSkill(a_actorValue, cost);
	}

public:
	void TryIncreaseSkill(RE::ActorValue a_actorValue)
	{
		if (GetStatBaseValue(a_actorValue) >= 100.0f)
		{
			auto message = std::format("You cannot increase {} further.", a_actorValue);
			return RE::DebugMessageBox(message.c_str());
		}

		switch (a_actorValue)
		{
		case RE::ActorValue::kOneHanded:
		case RE::ActorValue::kTwoHanded:
		case RE::ActorValue::kArchery:
		case RE::ActorValue::kBlock:
		case RE::ActorValue::kHeavyArmor:
		case RE::ActorValue::kLightArmor:
		case RE::ActorValue::kSneak:
		case RE::ActorValue::kAlteration:
		case RE::ActorValue::kConjuration:
		case RE::ActorValue::kDestruction:
		case RE::ActorValue::kIllusion:
		case RE::ActorValue::kRestoration:
			TryIncreaseLearningSkill(a_actorValue);
			break;
		case RE::ActorValue::kSmithing:
		case RE::ActorValue::kPickpocket:
		case RE::ActorValue::kLockpicking:
		case RE::ActorValue::kAlchemy:
		case RE::ActorValue::kSpeech:
		case RE::ActorValue::kEnchanting:
			TryIncreaseCraftingSkill(a_actorValue);
			break;
		}
	}

	void TryIncreaseSkill(std::int32_t a_actorValue)
	{
		TryIncreaseSkill(static_cast<RE::ActorValue>(a_actorValue));
	}

private:
	void UpdateBookValue(std::int32_t* a_value, std::array<std::uint32_t, kBookTotal> a_forms)
	{
		for (auto i = 0; i < kBookTotal; i++)
		{
			if (auto form = RE::TESForm::LookupByID(a_forms[i]);
				form && form->Is(RE::FormType::AlchemyItem))
			{
				a_value[i] = form->GetGoldValue();
			}
		}
	}

	void UpdateBookValues()
	{
		UpdateBookValue(bookValue[0], Forms_OneHanded);
		UpdateBookValue(bookValue[1], Forms_TwoHanded);
		UpdateBookValue(bookValue[2], Forms_Marksman);
		UpdateBookValue(bookValue[3], Forms_Block);
		UpdateBookValue(bookValue[4], Forms_Smithing);
		UpdateBookValue(bookValue[5], Forms_HeavyArmor);
		UpdateBookValue(bookValue[6], Forms_LightArmor);
		UpdateBookValue(bookValue[7], Forms_Pickpocket);
		UpdateBookValue(bookValue[8], Forms_Lockpicking);
		UpdateBookValue(bookValue[9], Forms_Speech);
		UpdateBookValue(bookValue[10], Forms_Alchemy);
		UpdateBookValue(bookValue[11], Forms_Sneak);
		UpdateBookValue(bookValue[12], Forms_Alteration);
		UpdateBookValue(bookValue[13], Forms_Conjuration);
		UpdateBookValue(bookValue[14], Forms_Destruction);
		UpdateBookValue(bookValue[15], Forms_Illusion);
		UpdateBookValue(bookValue[16], Forms_Restoration);
		UpdateBookValue(bookValue[17], Forms_Enchanting);
	}

private:
	void UpdateMenuName()
	{
		playerNameGold = std::format("{} / Pennies: {}", playerName, playerGold);
	}

public:
	void UpdateMenuName(RE::GPtr<RE::GFxMovieView>& a_view)
	{
		UpdateMenuName();
		{
			RE::GFxValue args[2];
			args[0] = playerNameGold;
			args[1] = playerClass;
			a_view->Invoke("heromenu_mc.SetStringValues", nullptr, args, 2);
		}
	}

private:
	float CalculateNeededExpPoints(float a_level, float a_slope, float a_mult, float a_expAcc = 1.0f, float a_expAcc20 = 1.2f, float a_expAcc30 = 1.5f, float a_expAcc40 = 2.0f)
	{
		auto result = powf(std::min(a_level, 20.0f), a_slope) * a_mult * a_expAcc;
		if (a_level <= 20)
			return result;

		result += (powf(std::min(a_level, 30.0f), a_slope) - powf(20.0f, a_slope)) * a_mult * a_expAcc * a_expAcc20;
		if (a_level <= 30)
			return result;

		result += (powf(std::min(a_level, 40.0f), a_slope) - powf(30.0f, a_slope)) * a_mult * a_expAcc * a_expAcc30;
		if (a_level <= 40)
			return result;

		result += (powf(a_level, a_slope) - powf(40.0f, a_slope)) * a_mult * a_expAcc * a_expAcc40;
		return result;
	}

private:
	void UpdateMenuData()
	{
		auto ui = RE::UI::GetSingleton();
		if (!ui)
			return;

		auto view = ui->GetMovieView("CustomMenu"sv);
		if (!view)
			return;

		// SetStringValues
		UpdateMenuName(view);

		// SetIntValues
		{
			RE::GFxValue args[33];
			args[0] = GetAttrBaseValue(RE::ActorValue::kHealth);
			args[1] = GetAttrValue(RE::ActorValue::kHealth);
			args[2] = GetAttrBaseValue(RE::ActorValue::kMagicka);
			args[3] = GetAttrValue(RE::ActorValue::kMagicka);
			args[4] = GetAttrBaseValue(RE::ActorValue::kStamina);
			args[5] = GetAttrValue(RE::ActorValue::kStamina);
			args[6] = -GetAttrValue(RE::ActorValue::kLastFlattered);
			args[7] = GetGlobalValue(kPlayerNeededXP);
			args[8] = GetGlobalValue(kPlayerXP);
			args[9] = GetGlobalValue(kPlayerLevel);
			args[10] = GetGlobalValue(kLearningPoints);
			args[11] = GetGlobalValue(kCraftingPoints);
			args[12] = GetGlobalValue(kTalentPoints);
			args[13] = GetStatBaseValue(RE::ActorValue::kIllusion);
			args[14] = GetStatBaseValue(RE::ActorValue::kDestruction);
			args[15] = GetStatBaseValue(RE::ActorValue::kAlteration);
			args[16] = GetStatBaseValue(RE::ActorValue::kOneHanded);
			args[17] = GetStatBaseValue(RE::ActorValue::kBlock);
			args[18] = GetStatBaseValue(RE::ActorValue::kArchery);
			args[19] = GetStatBaseValue(RE::ActorValue::kConjuration);
			args[20] = GetStatBaseValue(RE::ActorValue::kRestoration);
			args[21] = GetStatBaseValue(RE::ActorValue::kTwoHanded);
			args[22] = GetStatBaseValue(RE::ActorValue::kLightArmor);
			args[23] = GetStatBaseValue(RE::ActorValue::kHeavyArmor);
			args[24] = GetStatBaseValue(RE::ActorValue::kSneak);
			args[25] = GetStatBaseValue(RE::ActorValue::kAlchemy);
			args[26] = GetStatBaseValue(RE::ActorValue::kPickpocket);
			args[27] = GetStatBaseValue(RE::ActorValue::kLockpicking);
			args[28] = GetStatBaseValue(RE::ActorValue::kEnchanting);
			args[29] = GetStatBaseValue(RE::ActorValue::kSmithing);
			args[30] = GetStatBaseValue(RE::ActorValue::kSpeech);

			auto level = GetGlobalValue(kPlayerLevel);
			auto expMultSlope = GetGlobalValue(kExpMultSlope);
			auto expMult = GetGlobalValue(kExpMult);
			auto expNeededForCurrentLevel = CalculateNeededExpPoints(level - 1, expMultSlope, expMult);
			auto expNeededForNextLevel = CalculateNeededExpPoints(level, expMultSlope, expMult);

			args[31] = GetGlobalValue(kPlayerXP) - expNeededForCurrentLevel;
			args[32] = expNeededForNextLevel - expNeededForCurrentLevel;
			view->Invoke("heromenu_mc.SetIntValues", nullptr, args, 33);
		}

		// SetModifier
		{
			RE::GFxValue args[21];
			args[0] = GetAttrMod(RE::ActorValue::kHealth);
			args[1] = GetAttrMod(RE::ActorValue::kMagicka);
			args[2] = GetAttrMod(RE::ActorValue::kStamina);
			args[3] = GetStatMod(RE::ActorValue::kIllusion);
			args[4] = GetStatMod(RE::ActorValue::kDestruction);
			args[5] = GetStatMod(RE::ActorValue::kAlteration);
			args[6] = GetStatMod(RE::ActorValue::kOneHanded);
			args[7] = GetStatMod(RE::ActorValue::kBlock);
			args[8] = GetStatMod(RE::ActorValue::kArchery);
			args[9] = GetStatMod(RE::ActorValue::kConjuration);
			args[10] = GetStatMod(RE::ActorValue::kRestoration);
			args[11] = GetStatMod(RE::ActorValue::kTwoHanded);
			args[12] = GetStatMod(RE::ActorValue::kLightArmor);
			args[13] = GetStatMod(RE::ActorValue::kHeavyArmor);
			args[14] = GetStatMod(RE::ActorValue::kSneak);
			args[15] = GetStatMod(RE::ActorValue::kAlchemy);
			args[16] = GetStatMod(RE::ActorValue::kPickpocket);
			args[17] = GetStatMod(RE::ActorValue::kLockpicking);
			args[18] = GetStatMod(RE::ActorValue::kEnchanting);
			args[19] = GetStatMod(RE::ActorValue::kSmithing);
			args[20] = GetStatMod(RE::ActorValue::kSpeech);
			view->Invoke("heromenu_mc.SetModifier", nullptr, args, 21);
		}
	}

private:
	float        attrValue[4][kStatTotal]{};
	float        statValue[18][kStatTotal]{};
	std::int32_t bookValue[18][kBookTotal]{};
	std::string  playerName{};
	std::string  playerNameGold{};
	std::string  playerClass{};
	std::int32_t playerGold{};

private:
	RE::BGSPerk*       Seducer{ nullptr };
	RE::SpellItem*     Mesmerize01{ nullptr };
	RE::SpellItem*     Mesmerize02{ nullptr };
	RE::SpellItem*     Mesmerize03{ nullptr };
	RE::TESGlobal*     EXPMult{ nullptr };
	RE::TESGlobal*     EXPMultSlope{ nullptr };
	RE::TESGlobal*     PlayerXP{ nullptr };
	RE::TESGlobal*     PlayerNeededXP{ nullptr };
	RE::TESGlobal*     PlayerLevel{ nullptr };
	RE::TESGlobal*     CraftingPoints{ nullptr };
	RE::TESGlobal*     LearningPoints{ nullptr };
	RE::TESGlobal*     TalentPoints{ nullptr };
	RE::TESObjectMISC* Gold001{ nullptr };

private:
	bool overrideMessage{ false };

private:
	void UpdateForms()
	{
		if (auto data = RE::TESDataHandler::GetSingleton())
		{
			// clang-format off
			Seducer        = data->LookupForm<RE::BGSPerk>(0x069D3D, "Skyrim.esm"sv);
			Mesmerize01    = data->LookupForm<RE::SpellItem>(0x01EFDA, "Enderal - Forgotten Stories.esm"sv);
			Mesmerize02    = data->LookupForm<RE::SpellItem>(0x01EFDD, "Enderal - Forgotten Stories.esm"sv);
			Mesmerize03    = data->LookupForm<RE::SpellItem>(0x01EFDE, "Enderal - Forgotten Stories.esm"sv);
			EXPMult        = data->LookupForm<RE::TESGlobal>(0x008D2B, "Skyrim.esm"sv);
			EXPMultSlope   = data->LookupForm<RE::TESGlobal>(0x0D0EDB, "Skyrim.esm"sv);
			PlayerXP       = data->LookupForm<RE::TESGlobal>(0x012596, "Skyrim.esm"sv);
			PlayerNeededXP = data->LookupForm<RE::TESGlobal>(0x027CD1, "Skyrim.esm"sv);
			PlayerLevel    = data->LookupForm<RE::TESGlobal>(0x012595, "Skyrim.esm"sv);
			CraftingPoints = data->LookupForm<RE::TESGlobal>(0x085A79, "Skyrim.esm"sv);
			LearningPoints = data->LookupForm<RE::TESGlobal>(0x031ACB, "Skyrim.esm"sv);
			TalentPoints   = data->LookupForm<RE::TESGlobal>(0x05BCFA, "Skyrim.esm"sv);
			Gold001        = data->LookupForm<RE::TESObjectMISC>(0x00000F, "Skyrim.esm"sv);
			// clang-format on
		}
	}

private:
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_OneHanded{ 0x031ACC, 0x031ACE, 0x033A5F, 0x039935 };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_TwoHanded{ 0x039936, 0x039937, 0x039938, 0x039939 };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Marksman{ 0x085641, 0x085643, 0x085644, 0x085642 };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Block{ 0x039941, 0x039942, 0x039943, 0x039944 };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Smithing{ 0x0E7632, 0x0E7633, 0x0E7634, 0x0E7635 };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_HeavyArmor{ 0x03F86A, 0x03F86B, 0x03F86C, 0x03F86D };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_LightArmor{ 0x0E75F3, 0x0E75F4, 0x0E75F0, 0x0E75F2 };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Pickpocket{ 0x0E762E, 0x0E762F, 0x0E7630, 0x0E7631 };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Lockpicking{ 0x0E762A, 0x0E762B, 0x0E762C, 0x0E762D };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Speech{ 0x0E7636, 0x0E7637, 0x0E7638, 0x0E7639 };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Alchemy{ 0x0E7622, 0x0E7623, 0x0E7624, 0x0E7625 };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Sneak{ 0x08591F, 0x08591E, 0x08591D, 0x08591B };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Alteration{ 0x085618, 0x085619, 0x08561A, 0x08561B };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Conjuration{ 0x085621, 0x085622, 0x085620, 0x085623 };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Destruction{ 0x085614, 0x085615, 0x085616, 0x085617 };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Illusion{ 0x08561C, 0x08561D, 0x08561E, 0x08561F };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Restoration{ 0x085624, 0x085625, 0x085626, 0x085627 };
	static constexpr std::array<std::uint32_t, kBookTotal> Forms_Enchanting{ 0x0E7626, 0x0E7627, 0x0E7628, 0x0E7629 };
};
