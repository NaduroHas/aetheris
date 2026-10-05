// Copyright © 2026 AETHERIS. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AetherisCore.h"
#include "AetherisSimulation.h"
#include "GameFramework/Character.h"
#include "AetherisGameplay.generated.h"

// Player role types
UENUM(BlueprintType)
enum class EAetherisRole : uint8
{
	NoRole,
	Leader,
	Worker,
	Scout,
	Builder,
	Merchant,
	Explorer,
	Defender,
};

// Player ability types
UENUM(BlueprintType)
enum class EAetherisAbility : uint8
{
	NoAbility,
	Building,
	Crafting,
	Trading,
	Farming,
	Hunting,
	Exploring,
	Combat,
	Diplomacy,
};

// Aetheris Game Mode base
UCLASS()
class UEATHERISGAMEPLAY_API AAetherisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAetherisGameModeBase();

	// Get the simulation manager
	UFUNCTION(BlueprintCallable, Category = "Simulation")
	UAetherisSimulationManager* GetSimulationManager() const { return SimulationManager; }

	// Get the world manager
	UFUNCTION(BlueprintCallable, Category = "World")
	UAetherisWorld* GetWorldManager() const { return WorldManager; }

	// Spawn a player character at position
	UFUNCTION(BlueprintCallable, Category = "Game")
	AGameCharacter* SpawnPlayerCharacter(float X, float Y, float Z);

private:
	UPROPERTY()
	UAetherisSimulationManager* SimulationManager;

	UPROPERTY()
	UAetherisWorld* WorldManager;
};

// Aetheris Game Character — base for all entities
UCLASS()
class UEATHERISGAMEPLAY_API AGameCharacter : public ACharacter, public IAetherisTickable, public IAetherisEventListener
{
	GENERATED_BODY()

public:
	AGameCharacter();

	// Entity ID (64-bit deterministic)
	UPROPERTY(BlueprintReadOnly, Category = "Entity")
	uint64 EntityId;

	// Entity type (e.g., "Settlement", "Unit", "Player")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity")
	FString EntityType;

	// Current LOD level for this entity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
	EAetherisLOD CurrentLOD;

	// Role of this entity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	EAetherisRole CurrentRole;

	// Abilities available to this entity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TArray<EAetherisAbility> AvailableAbilities;

	// Name display
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	FString DisplayName;

	// Override: Simulation tick
	virtual void TickSimulation(float DeltaTime) override;

	// Override: Event listener
	virtual void OnAetherisEvent(const EAetherisEventType EventType, const FString& Details) override;

	// Get role as string
	UFUNCTION(BlueprintCallable, Category = "Role")
	FString GetRoleString() const;

	// Add a role
	UFUNCTION(BlueprintCallable, Category = "Role")
	void AddRole(EAetherisRole Role);

	// Remove a role
	UFUNCTION(BlueprintCallable, Category = "Role")
	void RemoveRole(EAetherisRole Role);
};

// Aetheris Player Controller — handles player input and God tools
UCLASS()
class UEATHERISGAMEPLAY_API AAetherisPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAetherisPlayerController();

	// God tool: Select entity by ID
	UFUNCTION(BlueprintCallable, Category = "GodTools")
	void SelectEntityById(uint64 EntityId);

	// God tool: Select entity under cursor
	UFUNCTION(BlueprintCallable, Category = "GodTools")
	void SelectEntityUnderCursor();

	// God tool: Inspect current selection
	UFUNCTION(BlueprintCallable, Category = "GodTools")
	void InspectSelectedEntity();

private:
	uint64 SelectedEntityId;
};
