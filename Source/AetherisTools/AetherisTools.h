// Copyright © 2026 AETHERIS. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AetherisCore.h"
#include "AetherisSimulation.h"
#include "EditorFramework/AnimCommand.h"
#include "AetherisTools.generated.h"

// Debug visualization levels
UENUM(BlueprintType)
enum class AetherisDebugLevel : uint8
{
	None,
	Minimal,    // Only basic entity info
	Standard,   // + needs, relationships, decisions
	Detailed,   // + knowledge provenance, decision trace
	Inspector,  // + full state, skills, event timeline
};

// Aetheris Debug Draw Component — visualizes simulation state
UCLASS()
class UEATHERISTOOLS_API UAetherisDebugComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAetherisDebugComponent();

	// Debug visualization level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	AetherisDebugLevel DebugLevel;

	// Show/hide entity info
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void ToggleDebugInfo();

	// Draw debug line from entity to target
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DrawRelationshipLine(const FLinearColor& Color);

	// Draw entity bounding box debug
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DrawEntityBounds();

private:
	bool bShowDebug;
};

// Aetheris Unit Inspector Widget — UI for inspecting individual entities
UCLASS()
class UEATHERISTOOLS_API UAetherisUnitInspector : public UObject
{
	GENERATED_BODY()

public:
	UAetherisUnitInspector();

	// Update inspector with entity data
	UFUNCTION(BlueprintCallable, Category = "Inspector")
	void UpdateFromEntity(const FString& Name, uint64 EntityId, EAetherisRole Role);

	// Get current inspected entity ID
	UFUNCTION(BlueprintCallable, Category = "Inspector")
	uint64 GetSelectedEntityId() const { return SelectedEntityId; }

private:
	uint64 SelectedEntityId;
};

// Aetheris Decision Trace — tracks why a decision was made
UCLASS()
class UEATHERISTOOLS_API UAetherisDecisionTrace : public UObject
{
	GENERATED_BODY()

public:
	UAetherisDecisionTrace();

	// Record a decision event
	UFUNCTION(BlueprintCallable, Category = "Trace")
	void RecordDecision(const FString& Action, const FString& Reason, float UtilityScore);

	// Record a candidate evaluation
	UFUNCTION(BlueprintCallable, Category = "Trace")
	void RecordCandidate(const FString& Candidate, float UtilityScore);

	// Get trace as string
	UFUNCTION(BlueprintCallable, Category = "Trace")
	FString GetTraceLog() const { return TraceLog; }

private:
	FString TraceLog;
};

// Aetheris Simulation Control Panel — UI for speed/step/replay
UCLASS()
class UEATHERISTOOLS_API UAetherisSimulationControlPanel : public UObject
{
	GENERATED_BODY()

public:
	UAetherisSimulationControlPanel();

	// Step forward one frame
	UFUNCTION(BlueprintCallable, Category = "Control")
	void StepForward(float DeltaTime);

	// Set speed preset (1x, 5x, 20x, 100x)
	UFUNCTION(BlueprintCallable, Category = "Control")
	void SetSpeedPreset(int32 PresetIndex);

	// Toggle pause
	UFUNCTION(BlueprintCallable, Category = "Control")
	void TogglePause();

private:
	float CurrentSpeed;
	bool bIsPaused;
};

// Aetheris Save/Load Manager
UCLASS()
class UEATHERISTOOLS_API UAetherisSaveManager : public UObject
{
	GENERATED_BODY()

public:
	UAetherisSaveManager();

	// Save simulation state to file
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	bool SaveSimulation(const FString& Filename);

	// Load simulation state from file
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	bool LoadSimulation(const FString& Filename);
};
