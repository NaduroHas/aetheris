// Copyright © 2026 AETHERIS. All rights reserved.

#include "AetherisIndividualSystems.h"

namespace
{
	static FAetherisSkillState* FindSkill(TArray<FAetherisSkillState>& Skills, const FName SkillId)
	{
		for (FAetherisSkillState& Skill : Skills)
		{
			if (Skill.SkillId == SkillId)
			{
				return &Skill;
			}
		}
		return nullptr;
	}

	static const FAetherisSkillState* FindSkill(const TArray<FAetherisSkillState>& Skills, const FName SkillId)
	{
		for (const FAetherisSkillState& Skill : Skills)
		{
			if (Skill.SkillId == SkillId)
			{
				return &Skill;
			}
		}
		return nullptr;
	}
}

void UAetherisIndividualSystems::AddSkillXP(FName SkillId, float Amount, float LearningMultiplier)
{
	if (SkillId.IsNone() || Amount <= 0.0f)
	{
		return;
	}

	FAetherisSkillState* Skill = FindSkill(Skills, SkillId);
	if (Skill == nullptr)
	{
		FAetherisSkillState NewSkill;
		NewSkill.SkillId = SkillId;
		NewSkill.XP = 0.0f;
		NewSkill.Level = 0.0f;
		NewSkill.Potential = 1.0f;
		Skills.Add(NewSkill);
		Skill = &Skills.Last();
	}

	const float SafeMultiplier = FMath::Max(0.0f, LearningMultiplier);
	Skill->XP += Amount * SafeMultiplier;

	// Lightweight, monotonic mastery curve. No automatic skill decay.
	const float RawLevel = FMath::Sqrt(FMath::Max(0.0f, Skill->XP) / 100.0f);
	Skill->Level = FMath::Min(Skill->Potential, RawLevel);
}

float UAetherisIndividualSystems::GetSkillLevel(FName SkillId) const
{
	if (const FAetherisSkillState* Skill = FindSkill(Skills, SkillId))
	{
		return Skill->Level;
	}
	return 0.0f;
}

float UAetherisIndividualSystems::GetSkillPotential(FName SkillId) const
{
	if (const FAetherisSkillState* Skill = FindSkill(Skills, SkillId))
	{
		return Skill->Potential;
	}
	return 0.0f;
}

void UAetherisIndividualSystems::AddKnowledge(const FAetherisKnowledge& Entry)
{
	if (Entry.KnowledgeId == 0 || Entry.Subject.IsNone())
	{
		return;
	}

	Knowledge.Add(Entry);
}

void UAetherisIndividualSystems::AddGoal(const FAetherisGoal& Goal)
{
	if (Goal.GoalId.IsNone())
	{
		return;
	}

	Goals.Add(Goal);
}

float UAetherisIndividualSystems::GetGoalPressure() const
{
	float Pressure = 0.0f;

	for (const FAetherisGoal& Goal : Goals)
	{
		const float Remaining = FMath::Clamp(1.0f - Goal.Progress, 0.0f, 1.0f);
		const float PriorityWeight = FMath::Clamp(static_cast<float>(Goal.Priority) / 100.0f, 0.0f, 1.0f);
		Pressure = FMath::Max(Pressure, Remaining * PriorityWeight);
	}

	return Pressure;
}

FAetherisActionScore UAetherisIndividualSystems::EvaluateAction(
	FName ActionId,
	float NeedValue,
	float GoalValue,
	float RelationshipValue,
	float ExpectedOutcome,
	float Risk,
	bool bHardAllowed) const
{
	FAetherisActionScore Result;
	Result.ActionId = ActionId;
	Result.bAllowed = bHardAllowed;

	if (!bHardAllowed)
	{
		Result.Utility = -1.0f;
		Result.Reason = TEXT("Rejected by hard constraint.");
		return Result;
	}

	const float SafeRisk = FMath::Clamp(Risk, 0.0f, 1.0f);
	const float RiskPreference = FMath::Lerp(1.0f - SafeRisk, 0.25f, Personality.RiskTolerance);
	const float Utility =
		(NeedValue * 0.35f) +
		(GoalValue * 0.25f) +
		(RelationshipValue * Personality.Loyalty * 0.15f) +
		(ExpectedOutcome * 0.25f * RiskPreference);

	Result.Utility = Utility;
	Result.Reason = FString::Printf(
		TEXT("Need %.2f | Goal %.2f | Relationship %.2f | Outcome %.2f | Risk %.2f"),
		NeedValue, GoalValue, RelationshipValue, ExpectedOutcome, SafeRisk);

	return Result;
}
