#pragma once

#include "Settings.h"
#include "Translator.h"

class Manager :
	public REX::TSingleton<Manager>
{
private:
	enum ActorValue : std::int32_t
	{
		// Skills
		kOneHanded,
		kTwoHanded,
		kMarksman,
		kBlock,
		kHandicraft,
		kHeavyArmor,
		kLightArmor,
		kSlightOfHand,
		kLockpicking,
		kSneak,
		kAlchemy,
		kRhetoric,
		kMentalism,
		kEntropy,
		kElementalism,
		kPsionics,
		kLightMagic,
		kEnchanting,

		// Attributes
		kHealth,
		kMana,
		kStamina,
		kArcaneFever,

		// Total
		ActorValueTotal,
		SkillTotal = 18,
	};

	enum Index : std::int32_t
	{
		kVal,
		kMin,
		kMax,
		IndexTotal,
	};

	enum Level : std::int32_t
	{
		kApprentice,
		kAdept,
		kExpert,
		kMaster,
		LevelTotal,
	};

	enum Global : std::int32_t
	{
		kExpMult,
		kExpMultSlope,
		kPlayerXP,
		kPlayerNeededXP,
		kPlayerLevel,
		kCraftingPoints,
		kLearningPoints,
		kTalentPoints,
		GlobalTotal,
	};

public:
	void LoadForms()
	{
		Translator::GetSingleton()->Load();
		UpdateForms();
		UpdateBookValues();
	}

	void LoadSave()
	{
		UpdateClassName();
		// UpdatePlayerName();
	}

	void UpdateCache()
	{
		UpdateClassName();
		UpdateActorValues();
		UpdatePlayerName();
		UpdatePlayerGold();
	}

public:
	bool QOverrideMessage() const
	{
		return overrideMessage;
	}

private:
	void SetOverrideMessage(bool a_value)
	{
		overrideMessage = a_value;
	}

private:
	auto ConvertActorValue(ActorValue a_av) const
	{
		switch (a_av)
		{
			case kOneHanded:
				return RE::ActorValue::kOneHanded;
			case kTwoHanded:
				return RE::ActorValue::kTwoHanded;
			case kMarksman:
				return RE::ActorValue::kArchery;
			case kBlock:
				return RE::ActorValue::kBlock;
			case kHandicraft:
				return RE::ActorValue::kSmithing;
			case kHeavyArmor:
				return RE::ActorValue::kHeavyArmor;
			case kLightArmor:
				return RE::ActorValue::kLightArmor;
			case kSlightOfHand:
				return RE::ActorValue::kPickpocket;
			case kLockpicking:
				return RE::ActorValue::kLockpicking;
			case kSneak:
				return RE::ActorValue::kSneak;
			case kAlchemy:
				return RE::ActorValue::kAlchemy;
			case kRhetoric:
				return RE::ActorValue::kSpeech;
			case kMentalism:
				return RE::ActorValue::kAlteration;
			case kEntropy:
				return RE::ActorValue::kConjuration;
			case kElementalism:
				return RE::ActorValue::kDestruction;
			case kPsionics:
				return RE::ActorValue::kIllusion;
			case kLightMagic:
				return RE::ActorValue::kRestoration;
			case kEnchanting:
				return RE::ActorValue::kEnchanting;
			case kHealth:
				return RE::ActorValue::kHealth;
			case kMana:
				return RE::ActorValue::kMagicka;
			case kStamina:
				return RE::ActorValue::kStamina;
			case kArcaneFever:
				return RE::ActorValue::kLastFlattered;
			default:
				return RE::ActorValue::kNone;
		}
	}

	auto ConvertActorValue(std::int32_t a_av) const
	{
		return ConvertActorValue(static_cast<ActorValue>(a_av));
	}

private:
	auto GetGlobalValue(Global a_global) const
	{
		switch (a_global)
		{
		case Global::kExpMult:
			return EXPMult ? EXPMult->value : 0.0f;
		case Global::kExpMultSlope:
			return EXPMultSlope ? EXPMultSlope->value : 0.0f;
		case Global::kPlayerXP:
			return PlayerXP ? PlayerXP->value : 0.0f;
		case Global::kPlayerNeededXP:
			return PlayerNeededXP ? PlayerNeededXP->value : 0.0f;
		case Global::kPlayerLevel:
			return PlayerLevel ? PlayerLevel->value : 0.0f;
		case Global::kCraftingPoints:
			return CraftingPoints ? CraftingPoints->value : 0.0f;
		case Global::kLearningPoints:
			return LearningPoints ? LearningPoints->value : 0.0f;
		case Global::kTalentPoints:
			return TalentPoints ? TalentPoints->value : 0.0f;
		default:
			return 0.0f;
		}
	}

private:
	auto GetActorValue(ActorValue a_av) const
	{
		return actorValue[a_av][kVal];
	}

	auto GetBaseActorValue(ActorValue a_av) const
	{
		return actorValue[a_av][kMin];
	}

	auto GetActorValueMod(ActorValue a_av) const
	{
		switch (a_av)
		{
		case kHealth:
		case kMana:
		case kStamina:
			return actorValue[a_av][kMax] - actorValue[a_av][kMin];
		default:
			return actorValue[a_av][kVal] - actorValue[a_av][kMin];
		}
	}

	void UpdateActorValue(ActorValue a_av)
	{
		if (auto player = RE::PlayerCharacter::GetSingleton())
		{
			auto av = ConvertActorValue(a_av);
			actorValue[a_av][kVal] = player->GetActorValue(av);
			actorValue[a_av][kMin] = player->GetBaseActorValue(av);
			actorValue[a_av][kMax] = player->GetActorValueMax(av);
		}
	}

	void UpdateActorValues()
	{
		if (auto player = RE::PlayerCharacter::GetSingleton())
		{
			for (auto i = 0; i < ActorValueTotal; i++)
			{
				auto av = ConvertActorValue(i);
				actorValue[i][kVal] = player->GetActorValue(av);
				actorValue[i][kMin] = player->GetBaseActorValue(av);
				actorValue[i][kMax] = player->GetActorValueMax(av);
			}
		}
	}

private:
	std::string_view GetActorValueName(ActorValue a_av) const
	{
		return Translator::GetSingleton()->GetActorValueName(a_av);
	}

	std::string_view GetGoldName() const
	{
		return Translator::GetSingleton()->GetGoldName();
	}

	std::string_view GetPointName(bool a_craft) const
	{
		return Translator::GetSingleton()->GetPointName(a_craft);
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

private:
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

	void UpdateClassName()
	{
		RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback{ new UpdateClassNameCallback };
		if (auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton())
			vm->DispatchStaticCall("EnderalFunctions", "GetPlayerClassNameGlobal", RE::MakeFunctionArguments(), callback);
	}

private:
	std::int32_t CalculateGoldCost(std::int32_t a_cost)
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

			mod = std::max(bmi, max - (max - min) * (std::min(GetActorValue(kRhetoric), 100.0f) / 100.0f));
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
					mod *= 1.0f + ((GetActorValue(kPsionics) / 6.0f) + 14.0f) / 100.0f;
				else if (Mesmerize02 && player->HasSpell(Mesmerize02))
					mod *= 1.0f + ((GetActorValue(kPsionics) / 6.0f) + 10.0f) / 100.0f;
				else if (Mesmerize01 && player->HasSpell(Mesmerize01))
					mod *= 1.0f + ((GetActorValue(kPsionics) / 6.0f) + 07.0f) / 100.0f;
			}

			mod = std::max(std::max(mod, bml), 1.0f);
		}

		return static_cast<std::int32_t>(roundf(a_cost * mod));
	}

	std::int32_t GetGoldCost(ActorValue a_av, Level a_idx)
	{
		return CalculateGoldCost(bookCost[a_av][a_idx]);
	}

	std::int32_t GetGoldCost(ActorValue a_av)
	{
		auto value = GetActorValue(a_av);
		if (value < 25)
			return GetGoldCost(a_av, kApprentice);
		else if (value < 50)
			return GetGoldCost(a_av, kAdept);
		else if (value < 75)
			return GetGoldCost(a_av, kExpert);
		else
			return GetGoldCost(a_av, kMaster);
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

	void ModGlobal(RE::TESGlobal* a_global, float a_mod)
	{
		if (a_global)
			a_global->value -= a_mod;
	}

	void ModValue(ActorValue a_av, float a_mod)
	{
		if (auto player = RE::PlayerCharacter::GetSingleton())
			player->ModBaseActorValue(ConvertActorValue(a_av), a_mod);
		UpdateActorValue(a_av);
	}

private:
	void IncreaseLearningSkill(ActorValue a_av, std::int32_t a_cost)
	{
		if (!Settings::GetSingleton()->IgnoreLearningGold)
			ModGold(a_cost);
		if (!Settings::GetSingleton()->IgnoreLearningPoints)
			ModGlobal(LearningPoints, 1.0f);
		ModValue(a_av, 1.0f);
		UpdateMenu();
	}

	void IncreaseCraftingSkill(ActorValue a_av, std::int32_t a_cost)
	{
		if (!Settings::GetSingleton()->IgnoreCraftingGold)
			ModGold(a_cost);
		if (!Settings::GetSingleton()->IgnoreCraftingPoints)
			ModGlobal(CraftingPoints, 1.0f);
		ModValue(a_av, 1.0f);
		UpdateMenu();
	}

	void IncreaseSkill(ActorValue a_av, std::int32_t a_cost)
	{
		switch (a_av)
		{
		case kOneHanded:
		case kTwoHanded:
		case kMarksman:
		case kBlock:
		case kHeavyArmor:
		case kLightArmor:
		case kSneak:
		case kMentalism:
		case kEntropy:
		case kElementalism:
		case kPsionics:
		case kLightMagic:
			IncreaseLearningSkill(a_av, a_cost);
			break;
		case kHandicraft:
		case kSlightOfHand:
		case kLockpicking:
		case kAlchemy:
		case kRhetoric:
		case kEnchanting:
			IncreaseCraftingSkill(a_av, a_cost);
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
		HeroMessageBoxCallback(ActorValue a_av, std::int32_t a_cost) :
			m_actorValue(a_av),
			m_cost(a_cost)
		{
			Manager::GetSingleton()->SetOverrideMessage(true);
		}

		virtual void Run(std::uint8_t a_button) override
		{
			if (a_button == 0)
				Manager::GetSingleton()->IncreaseSkill(m_actorValue, m_cost);
			Manager::GetSingleton()->SetOverrideMessage(false);
		}

	private:
		ActorValue   m_actorValue{};
		std::int32_t m_cost{};
	};

	void RequestCallback(ActorValue a_av, std::int32_t a_cost, bool a_crafting)
	{
		// clang-format off
		auto text = std::format(
			"Do you want to increase {0}?\n\nPrice: {1}p, 1 {2}\n\nCurrent Pennies: {3}\n Current {2}: {4}"sv,
			GetActorValueName(a_av),
			a_cost,
			GetPointName(a_crafting),
			playerGold,
			(a_crafting ? GetGlobalValue(kCraftingPoints) : GetGlobalValue(kLearningPoints)));

		RE::BSString message{ text };
		RE::BSTSmartPointer<RE::IMessageBoxCallback> call{ new HeroMessageBoxCallback(a_av, a_cost) };
		RE::MessageBoxMenu::Create(message, call, 0, 25, 10, HeroMessageBoxCallback::Buttons);
		// clang-format on
	}

	void TryIncreaseLearningSkill(ActorValue a_av)
	{
		auto cost = GetGoldCost(a_av);
		if (!Settings::GetSingleton()->IgnoreLearningGold &&
			cost > playerGold)
		{
			auto message = std::format(
				"You do not have enough pennies to increase {0}!\nPrice: {1}p"sv,
				GetActorValueName(a_av),
				cost);
			return RE::DebugMessageBox(message.c_str());
		}

		if (!Settings::GetSingleton()->IgnoreLearningPoints &&
			!GetGlobalValue(kLearningPoints))
		{
			auto message = std::format(
				"You do not have enough {0} to increase {1}!"sv, 
				GetPointName(false),
				GetActorValueName(a_av));
			return RE::DebugMessageBox(message.c_str());
		}

		if (!Settings::GetSingleton()->SkipCallback)
			RequestCallback(a_av, cost, false);
		else
			IncreaseLearningSkill(a_av, cost);
	}

	void TryIncreaseCraftingSkill(ActorValue a_av)
	{
		auto cost = GetGoldCost(a_av);
		if (!Settings::GetSingleton()->IgnoreCraftingGold &&
			cost > playerGold)
		{
			auto message = std::format(
				"You do not have enough pennies to increase {0}!\nPrice: {1}p"sv,
				GetActorValueName(a_av),
				cost);
			return RE::DebugMessageBox(message.c_str());
		}

		if (!Settings::GetSingleton()->IgnoreCraftingPoints &&
			!GetGlobalValue(kCraftingPoints))
		{
			auto message = std::format(
				"You do not have enough {0} to increase {1}!"sv,
				GetPointName(true),
				GetActorValueName(a_av));
			return RE::DebugMessageBox(message.c_str());
		}

		if (!Settings::GetSingleton()->SkipCallback)
			RequestCallback(a_av, cost, true);
		else
			IncreaseCraftingSkill(a_av, cost);
	}

public:
	void TryIncreaseSkill(ActorValue a_av)
	{
		if (GetBaseActorValue(a_av) >= 100.0f)
		{
			auto message = std::format(
				"You cannot increase {0} further.",
				GetActorValueName(a_av));
			return RE::DebugMessageBox(message.c_str());
		}

		switch (a_av)
		{
		case kOneHanded:
		case kTwoHanded:
		case kMarksman:
		case kBlock:
		case kHeavyArmor:
		case kLightArmor:
		case kSneak:
		case kMentalism:
		case kEntropy:
		case kElementalism:
		case kPsionics:
		case kLightMagic:
			TryIncreaseLearningSkill(a_av);
			break;
		case kHandicraft:
		case kSlightOfHand:
		case kLockpicking:
		case kAlchemy:
		case kRhetoric:
		case kEnchanting:
			TryIncreaseCraftingSkill(a_av);
			break;
		}
	}

	void TryIncreaseSkill(std::int32_t a_av)
	{
		TryIncreaseSkill(static_cast<ActorValue>(a_av));
	}

private:
	void UpdateBookValue(ActorValue a_av)
	{
		for (std::int32_t i = 0; i < LevelTotal; i++)
		{
			if (auto form = RE::TESForm::LookupByID(bookForm[a_av][i]))
				bookCost[a_av][i] = form->GetGoldValue();
		}
	}

	void UpdateBookValues()
	{
		UpdateBookValue(kOneHanded);
		UpdateBookValue(kTwoHanded);
		UpdateBookValue(kMarksman);
		UpdateBookValue(kBlock);
		UpdateBookValue(kHandicraft);
		UpdateBookValue(kHeavyArmor);
		UpdateBookValue(kLightArmor);
		UpdateBookValue(kSlightOfHand);
		UpdateBookValue(kLockpicking);
		UpdateBookValue(kRhetoric);
		UpdateBookValue(kAlchemy);
		UpdateBookValue(kSneak);
		UpdateBookValue(kMentalism);
		UpdateBookValue(kEntropy);
		UpdateBookValue(kElementalism);
		UpdateBookValue(kPsionics);
		UpdateBookValue(kLightMagic);
		UpdateBookValue(kEnchanting);
	}

private:
	void UpdateMenuName()
	{
		playerNameGold = std::format("{0} / {1}: {2}", playerName, GetGoldName(), playerGold);
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
	auto CalculateNeededExpPoints(float a_level, float a_slope, float a_mult, float a_expAcc = 1.0f, float a_expAcc20 = 1.2f, float a_expAcc30 = 1.5f, float a_expAcc40 = 2.0f)
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
	void UpdateMenu()
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

			auto expNeededForNextLevel = CalculateNeededExpPoints(GetGlobalValue(Global::kPlayerLevel), GetGlobalValue(Global::kExpMultSlope), GetGlobalValue(Global::kExpMult));
			auto expNeededForThisLevel = CalculateNeededExpPoints(GetGlobalValue(Global::kPlayerLevel) - 1, GetGlobalValue(Global::kExpMultSlope), GetGlobalValue(Global::kExpMult));

			// Health
			args[0] = GetBaseActorValue(kHealth);
			args[1] = GetActorValue(kHealth);

			// Mana
			args[2] = GetBaseActorValue(kMana);
			args[3] = GetActorValue(kMana);

			// Stamina
			args[4] = GetBaseActorValue(kStamina);
			args[5] = GetActorValue(kStamina);

			// Arcane Fever
			args[6] = -GetActorValue(kArcaneFever);

			// Globals
			args[7] = expNeededForNextLevel;
			args[8] = GetGlobalValue(kPlayerXP);
			args[9] = GetGlobalValue(kPlayerLevel);
			args[10] = GetGlobalValue(kLearningPoints);
			args[11] = GetGlobalValue(kCraftingPoints);
			args[12] = GetGlobalValue(kTalentPoints);

			// Skills
			args[13] = GetBaseActorValue(kPsionics);
			args[14] = GetBaseActorValue(kElementalism);
			args[15] = GetBaseActorValue(kMentalism);
			args[16] = GetBaseActorValue(kOneHanded);
			args[17] = GetBaseActorValue(kBlock);
			args[18] = GetBaseActorValue(kMarksman);
			args[19] = GetBaseActorValue(kEntropy);
			args[20] = GetBaseActorValue(kLightMagic);
			args[21] = GetBaseActorValue(kTwoHanded);
			args[22] = GetBaseActorValue(kLightArmor);
			args[23] = GetBaseActorValue(kHeavyArmor);
			args[24] = GetBaseActorValue(kSneak);
			args[25] = GetBaseActorValue(kAlchemy);
			args[26] = GetBaseActorValue(kSlightOfHand);
			args[27] = GetBaseActorValue(kLockpicking);
			args[28] = GetBaseActorValue(kEnchanting);
			args[29] = GetBaseActorValue(kHandicraft);
			args[30] = GetBaseActorValue(kRhetoric);

			// XP
			args[31] = GetGlobalValue(kPlayerXP) - expNeededForThisLevel;
			args[32] = expNeededForNextLevel - expNeededForThisLevel;
			view->Invoke("heromenu_mc.SetIntValues", nullptr, args, 33);
		}

		// SetModifier
		{
			RE::GFxValue args[21];
			// Attributes
			args[0] = GetActorValueMod(kHealth);
			args[1] = GetActorValueMod(kMana);
			args[2] = GetActorValueMod(kStamina);

			// Skills
			args[3] = GetActorValueMod(kPsionics);
			args[4] = GetActorValueMod(kElementalism);
			args[5] = GetActorValueMod(kMentalism);
			args[6] = GetActorValueMod(kOneHanded);
			args[7] = GetActorValueMod(kBlock);
			args[8] = GetActorValueMod(kMarksman);
			args[9] = GetActorValueMod(kEntropy);
			args[10] = GetActorValueMod(kLightMagic);
			args[11] = GetActorValueMod(kTwoHanded);
			args[12] = GetActorValueMod(kLightArmor);
			args[13] = GetActorValueMod(kHeavyArmor);
			args[14] = GetActorValueMod(kSneak);
			args[15] = GetActorValueMod(kAlchemy);
			args[16] = GetActorValueMod(kSlightOfHand);
			args[17] = GetActorValueMod(kLockpicking);
			args[18] = GetActorValueMod(kEnchanting);
			args[19] = GetActorValueMod(kHandicraft);
			args[20] = GetActorValueMod(kRhetoric);
			view->Invoke("heromenu_mc.SetModifier", nullptr, args, 21);
		}
	}

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
	float actorValue[ActorValueTotal][IndexTotal]{};

private:
	std::int32_t playerGold{};
	std::string  playerName{};
	std::string  playerNameGold{};
	std::string  playerClass{};

private:
	std::int32_t bookCost[SkillTotal][LevelTotal]{};

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
	static constexpr std::array<std::array<std::uint32_t, LevelTotal>, SkillTotal> bookForm{
		std::array<std::uint32_t, 4>{ 0x031ACC, 0x031ACE, 0x033A5F, 0x039935 },
		std::array<std::uint32_t, 4>{ 0x039936, 0x039937, 0x039938, 0x039939 },
		std::array<std::uint32_t, 4>{ 0x085641, 0x085643, 0x085644, 0x085642 },
		std::array<std::uint32_t, 4>{ 0x039941, 0x039942, 0x039943, 0x039944 },
		std::array<std::uint32_t, 4>{ 0x0E7632, 0x0E7633, 0x0E7634, 0x0E7635 },
		std::array<std::uint32_t, 4>{ 0x03F86A, 0x03F86B, 0x03F86C, 0x03F86D },
		std::array<std::uint32_t, 4>{ 0x0E75F3, 0x0E75F4, 0x0E75F0, 0x0E75F2 },
		std::array<std::uint32_t, 4>{ 0x0E762E, 0x0E762F, 0x0E7630, 0x0E7631 },
		std::array<std::uint32_t, 4>{ 0x0E762A, 0x0E762B, 0x0E762C, 0x0E762D },
		std::array<std::uint32_t, 4>{ 0x0E7636, 0x0E7637, 0x0E7638, 0x0E7639 },
		std::array<std::uint32_t, 4>{ 0x0E7622, 0x0E7623, 0x0E7624, 0x0E7625 },
		std::array<std::uint32_t, 4>{ 0x08591F, 0x08591E, 0x08591D, 0x08591B },
		std::array<std::uint32_t, 4>{ 0x085618, 0x085619, 0x08561A, 0x08561B },
		std::array<std::uint32_t, 4>{ 0x085621, 0x085622, 0x085620, 0x085623 },
		std::array<std::uint32_t, 4>{ 0x085614, 0x085615, 0x085616, 0x085617 },
		std::array<std::uint32_t, 4>{ 0x08561C, 0x08561D, 0x08561E, 0x08561F },
		std::array<std::uint32_t, 4>{ 0x085624, 0x085625, 0x085626, 0x085627 },
		std::array<std::uint32_t, 4>{ 0x0E7626, 0x0E7627, 0x0E7628, 0x0E7629 },
	};
};
