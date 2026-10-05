// Copyright © 2026 AETHERIS. All rights reserved.

#include "AetherisTools.h"
#include "AetherisCore.h"

// Debug Component
UAetherisDebugComponent::UAetherisDebugComponent()
	: DebugLevel(AetherisDebugLevel::Minimal)
	, bShowDebug(false)
{
}

void UAetherisDebugComponent::ToggleDebugInfo()
{
	bShowDebug = !bShowDebug;
	Aetheris::LogCore(FString::Printf(TEXT("[Debug] Debug info %s"), bShowDebug ? TEXT("ON") : TEXT("OFF")));
}

void UAetherisDebugComponent::DrawRelationshipLine(const FLinearColor& Color)
{
	// Placeholder: would draw a debug line in world space
}

void UAetherisDebugComponent::DrawEntityBounds()
{
	// Placeholder: would draw entity bounding box
}

// Unit Inspector
UAetherisUnitInspector::UAetherisUnitInspector()
	: SelectedEntityId(0)
{
}

void UAetherisUnitInspector::UpdateFromEntity(const FString& Name, uint64 EntityId, EAetherisRole Role)
{
	SelectedEntityId = EntityId;
	Aetheris::LogCore(FString::Printf(TEXT("[Inspector] Inspecting: %s (ID: %llu)"), *Name, EntityId));
}

// Decision Trace
UAetherisDecisionTrace::UAetherisDecisionTrace()
{
}

void UAetherisDecisionTrace::RecordDecision(const FString& Action, const FString& Reason, float UtilityScore)
{
	FString Entry = FString::Printf(TEXT("[DECISION] Action: %s | Reason: %s | Utility: %.3f"), *Action, *Reason, UtilityScore);
	TraceLog += Entry + TEXT("\n");
}

void UAetherisDecisionTrace::RecordCandidate(const FString& Candidate, float UtilityScore)
{
	FString Entry = FString::Printf(TEXT("[  CANDIDATE] %s: Utility %.3f"), *Candidate, UtilityScore);
	TraceLog += Entry + TEXT("\n");
}

// Simulation Control Panel
UAetherisSimulationControlPanel::UAetherisSimulationControlPanel()
	: CurrentSpeed(1.0f)
	, bIsPaused(false)
{
}

void UAetherisSimulationControlPanel::StepForward(float DeltaTime)
{
	Aetheris::LogCore(FString::Printf(TEXT("[StepForward] Stepping by %.2f"), DeltaTime));
}

void UAetherisSimulationControlPanel::SetSpeedPreset(int32 PresetIndex)
{
	switch (PresetIndex)
	{
	case 0: CurrentSpeed = 1.0f; break;
	case 1: CurrentSpeed = 5.0f; break;
	case 2: CurrentSpeed = 20.0f; break;
	case 3: CurrentSpeed = 100.0f; break;
	default: CurrentSpeed = 1.0f; break;
	}
}

void UAetherisSimulationControlPanel::TogglePause()
{
	bIsPaused = !bIsPaused;
	Aetheris::LogCore(FString::Printf(TEXT("[SpeedControl] %s"), bIsPaused ? TEXT("Paused") : TEXT("Running")));
}

// Save Manager
UAetherisSaveManager::UAetherisSaveManager()
{
}

bool UAetherisSaveManager::SaveSimulation(const FString& Filename)
{
	Aetheris::LogCore(FString::Printf(TEXT("[Save] Saving simulation to: %s"), *Filename));
	// Placeholder: actual save implementation would serialize simulation state
	return true;
}

bool UAetherisSaveManager::LoadSimulation(const FString& Filename)
{
	Aetheris::LogCore(FString::Printf(TEXT("[Load] Loading simulation from: %s"), *Filename));
	// Placeholder: actual load implementation would deserialize simulation state
	return true;
}
