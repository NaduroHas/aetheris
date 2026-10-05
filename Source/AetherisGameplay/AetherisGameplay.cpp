// Copyright © 2026 AETHERIS. All rights reserved.

#include "AetherisGameplay.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

static const TMap<EAetherisRole, EAetherisAbility> RoleAbilityMap =
{
	{ EAetherisRole::Leader, EAetherisAbility::Diplomacy },
	{ EAetherisRole::Worker, EAetherisAbility::Farming },
	{ EAetherisRole::Scout, EAetherisAbility::Exploring },
	{ EAetherisRole::Builder, EAetherisAbility::Building },
	{ EAetherisRole::Merchant, EAetherisAbility::Trading },
	{ EAetherisRole::Explorer, EAetherisAbility::Exploring },
	{ EAetherisRole::Defender, EAetherisAbility::Combat },
};

AAetherisGameModeBase::AAetherisGameModeBase()
{
	SimulationManager = NewObject<UAetherisSimulationManager>(this, TEXT("SimulationManager"));
	WorldManager = NewObject<UAetherisWorld>(this, TEXT("WorldManager"));
}

void AAetherisGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;

	if (SimulationManager)
	{
		SimulationManager->Initialize();
	}

	Aetheris::LogCore(TEXT("GameMode initialized — simulation loop active"));
}

void AAetherisGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SimulationManager)
	{
		SimulationManager->TickSimulation(DeltaTime);
	}
}

AGameCharacter* AAetherisGameModeBase::SpawnPlayerCharacter(float X, float Y, float Z)
{
	if (!GetWorld())
	{
		return nullptr;
	}

	if (AActor* PlayerStart = UGameplayStatics::GetPlayerStart(GetWorld(), 0))
	{
		X = PlayerStart->GetActorLocation().X;
		Y = PlayerStart->GetActorLocation().Y;
		Z = PlayerStart->GetActorLocation().Z;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AGameCharacter* Spawned = GetWorld()->SpawnActor<AGameCharacter>(
		AGameCharacter::StaticClass(),
		FVector(X, Y, Z),
		FRotator::ZeroRotator,
		Params);

	if (Spawned && SimulationManager)
	{
		SimulationManager->RegisterEntity(Spawned, TEXT("Player"), Spawned->EntityId);
		Aetheris::LogCore(TEXT("Player character spawned"));
	}

	return Spawned;
}

AGameCharacter::AGameCharacter()
{
	EntityId = Aetheris::GetEntityId(TEXT("Player"), 0);
	EntityType = TEXT("Character");
	CurrentLOD = EAetherisLOD::LOD0;
	CurrentRole = EAetherisRole::NoRole;
	PrimaryActorTick.bCanEverTick = true;
}

void AGameCharacter::TickSimulation(float DeltaTime)
{
}

void AGameCharacter::OnAetherisEvent(const EAetherisEventType EventType, const FString& Details)
{
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
			TEXT("[AddRole] No ability mapping for role %d"),
			static_cast<uint8>(Role)));
	}

	CurrentRole = Role;
}

void AGameCharacter::RemoveRole(EAetherisRole Role)
{
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

AAetherisPlayerController::AAetherisPlayerController()
	: SelectedEntityId(0)
{
}

void AAetherisPlayerController::SelectEntityById(uint64 InEntityId)
{
	SelectedEntityId = InEntityId;
	Aetheris::LogCore(FString::Printf(TEXT("[GodTool] Selected entity: %llu"), InEntityId));
}

void AAetherisPlayerController::SelectEntityUnderCursor()
{
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
