// Copyright © 2026 AETHERIS. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AetherisIndividualSystems.generated.h"

USTRUCT(BlueprintType)
struct UEATHERISSIMULATION_API FAetherisPersonality
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Personality", meta=(ClampMin="0.0", ClampMax="1.0"))
	float Courage = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Personality", meta=(ClampMin="0.0", ClampMax="1.0"))
	float Sociability = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Personality", meta=(ClampMin="0.0", ClampMax="1.0"))
	float Curiosity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Personality", meta=(ClampMin="0.0", ClampMax="1.0"))
	float Loyalty = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Personality", meta=(ClampMin="0.0", ClampMax="1.0"))
	float Discipline = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Personality", meta=(ClampMin="0.0", ClampMax="1.0"))
	float Empathy = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Personality", meta=(ClampMin="0.0", ClampMax="1.0"))
	float RiskTolerance = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Personality", meta=(ClampMin="0.0", ClampMax="1.0"))
	float SelfConfidence = 0.5f;
};

USTRUCT(BlueprintType)
struct UEATHERISSIMULATION_API FAetherisSkillState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
	FName SkillId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill", meta=(ClampMin="0.0"))
	float XP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill", meta=(ClampMin="0.0"))
	float Level = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill", meta=(ClampMin="0.0"))
	float Potential = 1.0f;
};

USTRUCT(BlueprintType)
struct UEATHERISSIMULATION_API FAetherisKnowledge
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Knowledge")
	uint64 KnowledgeId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Knowledge")
	FName Subject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Knowledge")
	FString Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Knowledge", meta=(ClampMin="0.0", ClampMax="1.0"))
	float Trust = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Knowledge", meta=(ClampMin="0.0", ClampMax="1.0"))
	float AccuracyBelief = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Knowledge")
	uint64 SourceUnitId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Knowledge")
	bool bPersonallyExperienced = false;
};

USTRUCT(BlueprintType)
struct UEATHERISSIMULATION_API FAetherisGoal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal")
	FName GoalId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal")
	int32 Priority = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal")
	float Progress = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goal")
	bool bLongTerm = false;
};

USTRUCT(BlueprintType)
struct UEATHERISSIMULATION_API FAetherisActionScore
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Decision")
	FName ActionId;

	UPROPERTY(BlueprintReadOnly, Category="Decision")
	float Utility = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Decision")
	bool bAllowed = false;

	UPROPERTY(BlueprintReadOnly, Category="Decision")
	FString Reason;
};

UCLASS(BlueprintType)
class UEATHERISSIMULATION_API UAetherisIndividualSystems : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Personality")
	FAetherisPersonality Personality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TArray<FAetherisSkillState> Skills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Knowledge")
	TArray<FAetherisKnowledge> Knowledge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Goals")
	TArray<FAetherisGoal> Goals;

	UFUNCTION(BlueprintCallable, Category="Skills")
	void AddSkillXP(FName SkillId, float Amount, float LearningMultiplier = 1.0f);

	UFUNCTION(BlueprintPure, Category="Skills")
	float GetSkillLevel(FName SkillId) const;

	UFUNCTION(BlueprintPure, Category="Skills")
	float GetSkillPotential(FName SkillId) const;

	UFUNCTION(BlueprintCallable, Category="Knowledge")
	void AddKnowledge(const FAetherisKnowledge& Entry);

	UFUNCTION(BlueprintCallable, Category="Goals")
	void AddGoal(const FAetherisGoal& Goal);

	UFUNCTION(BlueprintPure, Category="Goals")
	float GetGoalPressure() const;

	// Hard constraints reject impossible actions before utility is evaluated.
	UFUNCTION(BlueprintCallable, Category="Decision")
	FAetherisActionScore EvaluateAction(
		FName ActionId,
		float NeedValue,
		float GoalValue,
		float RelationshipValue,
		float ExpectedOutcome,
		float Risk,
		bool bHardAllowed) const;
};