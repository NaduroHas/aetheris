// Copyright © 2026 AETHERIS. All rights reserved.

#include "AetherisGameplay.h"
#include "AetherisSimulation.h"
#include "AetherisCore.h"

// GameMode
AAetherisGameModeBase::AAetherisGameModeBase()
{
	SimulationManager = CreateDefaultSubobject<UAetherisSimulationManager>(TEXT("SimulationManager"));
	WorldManager = CreateDefaultSubobject<UAetherisWorld>(TEXT("WorldManager"));
}

AGameCharacter* AAetherisGameModeBase::SpawnPlayerCharacter(float X, float Y, float Z)
{
	// Simple placeholder — in production this would use a PlayerCharacter BP
	return nullptr;
}

// GameCharacter
AGameCharacter::AGameCharacter()
{
	// Generate deterministic ID
	EntityId = Aetheris::GenerateSeedId(TEXT("Player_0"));
	EntityType = TEXT("Character");
	CurrentLOD = EAetherisLOD::LOD0;
	CurrentRole = EAetherisRole::NoRole;
}

void AGameCharacter::TickSimulation(float DeltaTime)
{
	// Placeholder: this is where entity-specific simulation logic goes
	// In production: needs update, pathfinding, decision making, etc.
}

void AGameCharacter::OnAetherisEvent(const EAetherisEventType EventType, const FString& Details)
{
	// Placeholder: entity responds to events
	// In production: handle specific event types
}

FString AGameCharacter::GetRoleString() const
{
	switch (CurrentRole)
	{
	case EAetherisRole::NoRole: return TEXT("None");
	case EAetherisRole::Leader: return TEXT("Leader");
	case EAetherisRole::Worker: return TEXT("Worker");
	case EAetherisRole::Scout: return TEXT("Scout");
	case EAetherisRole::Builder: return TEXT("Builder");
	case EAetherisRole::Merchant: return TEXT("Merchant");
	case EAetherisRole::Explorer: return TEXT("Explorer");
	case EAetherisRole::Defender: return TEXT("Defender");
	default: return TEXT("Unknown");
	}
}

void AGameCharacter::AddRole(EAetherisRole Role)
{
	if (!AvailableAbilities.Contains(static_cast<EAetherisAbility>(Role)))
	{
		AvailableAbilities.Add(static_cast<EAetherisAbility>(Role));
	}
	CurrentRole = Role;
}

void AGameCharacter::RemoveRole(EAetherisRole Role)
{
	AvailableAbilities.Remove(static_cast<EAetherisAbility>(Role));
}

// PlayerController
AAetherisPlayerController::AAetherisPlayerController()
	: SelectedEntityId(0)
{
}

void AAetherisPlayerController::SelectEntityById(uint64 EntityId)
{
	SelectedEntityId = EntityId;
	Aetheris::LogCore(FString::Printf(TEXT("[GodTool] Selected entity: %llu"), EntityId));
}

void AAetherisPlayerController::SelectEntityUnderCursor()
{
	// Placeholder: trace from camera, find actor at cursor
	Aetheris::LogCore(TEXT("[GodTool] Trace under cursor"));
}

void AAetherisPlayerController::InspectSelectedEntity()
{
	if (SelectedEntityId == 0)
	{
		Aetheris::LogWarning(TEXT("[GodTool] No entity selected"));
		return;
	}
	Aetheris::LogCore(FString::Printf(TEXT("[GodTool] Inspecting entity: %llu"), SelectedEntityId));
}
