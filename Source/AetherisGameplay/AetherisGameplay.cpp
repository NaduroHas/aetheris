// Copyright © 2026 AETHERIS. All rights reserved.

#include "AetherisGameplay.h"
#include "AetherisSimulation.h"
#include "AetherisCore.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Role → Ability mapping table (design-driven, @der-weltenbauer definiert)
static const TMap<EAetherisRole, EAetherisAbility> RoleAbilityMap =
{
	{ EAetherisRole::Leader,	EAetherisAbility::Diplomacy },
	{ EAetherisRole::Worker,	EAetherisAbility::Farming },
	{ EAetherisRole::Scout,		EAetherisAbility::Exploring },
	{ EAetherisRole::Builder,	EAetherisAbility::Building },
	{ EAetherisRole::Merchant,	EAetherisAbility::Trading },
	{ EAetherisRole::Explorer,	EAetherisAbility::Exploring },
	{ EAetherisRole::Defender,	EAetherisAbility::Combat },
};

// GameMode
AAetherisGameModeBase::AAetherisGameModeBase()
{
	// Fixed: Use NewObject<> for proper UPROPERTY management
	// CreateDefaultSubobject() on non-UPROPERTY pointers is broken (BUG #4)
	SimulationManager = NewObject<UAetherisSimulationManager>(this, TEXT("SimulationManager"));
	WorldManager = NewObject<UAetherisWorld>(this, TEXT("WorldManager"));

	// Set default subobject pointers for Blueprint access
	SetWorldPartitionWorldContext(SimulationManager);
}

void AAetherisGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Enable GameMode tick for simulation loop (BUG #7 fix)
	PrimaryActorTick.bCanEverTick = true;

	// Register SimulationManager as an entity if it has registered entities
	Aetheris::LogCore(TEXT("GameMode initialized — simulation loop active"));
}

void AAetherisGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Delegate to simulation manager (BUG #7 fix — the Game Loop)
	if (SimulationManager)
	{
		SimulationManager->TickSimulation(DeltaTime);
	}
}

AGameCharacter* AAetherisGameModeBase::SpawnPlayerCharacter(float X, float Y, float Z)
{
	// Fixed: Actual spawn instead of nullptr (BUG #5)
	if (!GetWorld()) return nullptr;

	// Try to spawn from a defined PlayerStart
	AActor* PlayerStart = UGameplayStatics::GetPlayerStart(GetWorld(), 0);
	if (PlayerStart)
	{
		X = PlayerStart->GetActorLocation().X;
		Y = PlayerStart->GetActorLocation().Y;
		Z = PlayerStart->GetActorLocation().Z;
	}

	// For now, spawn a basic GameCharacter placeholder
	// In production: use a BP-based player character class
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AGameCharacter* Spawned = GetWorld()->SpawnActor<AGameCharacter>(
		AGameCharacter::StaticClass(),
		FVector(X, Y, Z),
		FRotator::ZeroRotator,
		Params
	);

	if (Spawned)
	{
		// Register with simulation
		if (SimulationManager)
		{
			SimulationManager->RegisterEntity(Spawned, TEXT("Player"), Spawned->EntityId);
		}
		Aetheris::LogCore(TEXT("Player character spawned"));
	}

	return Spawned;
}

// GameCharacter
AGameCharacter::AGameCharacter()
{
	// Generate deterministic ID
	EntityId = Aetheris::GenerateSeedId(TEXT("Player_0"));
	EntityType = TEXT("Character");
	CurrentLOD = EAetherisLOD::LOD0;
	CurrentRole = EAetherisRole::NoRole;

	// Enable tick for simulation
	PrimaryActorTick.bCanEverTick = true;
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
	// Fixed: Use design-driven Role→Ability mapping table (BUG #6 fix)
	EAetherisAbility Ability;
	if (RoleAbilityMap.Find(Role, Ability))
	{
		if (!AvailableAbilities.Contains(Ability))
		{
			AvailableAbilities.Add(Ability);
		}
	}
	else
	{
		Aetheris::LogWarning(FString::Printf(
			TEXT("[AddRole] No ability mapping for role %s"),
			*GetRoleString()
		));
	}

	CurrentRole = Role;
}

void AGameCharacter::RemoveRole(EAetherisRole Role)
{
	// Remove the mapped ability
	EAetherisAbility Ability;
	if (RoleAbilityMap.Find(Role, Ability))
	{
		AvailableAbilities.Remove(Ability);
	}

	if (CurrentRole == Role)
	{
		CurrentRole = EAetherisRole::NoRole;
	}
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
