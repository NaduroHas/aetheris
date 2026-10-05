// Copyright © 2026 AETHERIS. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AetherisCore.h"
#include "GameFramework/Actor.h"
#include "AetherisSimulationCore.generated.h"

// LOD levels for simulation entities
UENUM(BlueprintType)
enum class EAetherisLOD : uint8
{
	LOD0 = 0,   // Full simulation (player visible, detailed)
	LOD1 = 1,   // Medium simulation
	LOD2 = 2,   // Low simulation
	LOD3 = 3,   // Mass simulation (batched)
	LOD4 = 4,   // Event-only simulation
};

// Core event types for the event bus
UENUM(BlueprintType)
enum class EAetherisEventType : uint8
{
	// Individual events
	OnNeedsChanged,
	OnStateChange,
	OnDecisionMade,
	OnSkillUsed,
	OnRelationshipChanged,

	// Social events
	OnRelationshipFormed,
	OnRelationshipBroken,
	OnGroupFormed,
	OnGroupDissolved,

	// World events
	OnResourceSpawned,
	OnResourceDepleted,
	OnWeatherChanged,
	OnSeasonChanged,
	OnTerrainChanged,

	// Economy events
	OnTradeOccurred,
	OnProductionOccurred,
	OnConsumptionOccurred,
	OnPriceChanged,

	// General
	OnSimulationStep,
	OnDebugEvent,
};

// Base simulation tick interface
UINTERFACE()
class UEATHERISSIMULATION_API UAetherisTickable : public UInterface
{
	GENERATED_BODY()
};

class IAetherisTickable
{
	GENERATED_BODY()

public:
	virtual void TickSimulation(float DeltaTime) = 0;
	virtual void PauseSimulation() {}
	virtual void ResumeSimulation() {}
};

// Aetheris World Manager — manages the simulation world, chunks, LOD boundaries
UCLASS()
class UEATHERISSIMULATION_API UAetherisWorld : public UObject
{
	GENERATED_BODY()

public:
	AetherisWorld() : WorldSizeX(2000.f), WorldSizeZ(2000.f), ChunkSize(500.f) {}

	// World dimensions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
	float WorldSizeX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
	float WorldSizeZ;

	// Chunk size for spatial partitioning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
	float ChunkSize;

	// Current simulation speed multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
	float SimulationSpeed = 1.0f;

	// Current simulation state
	UPROPERTY(BlueprintReadOnly, Category = "Simulation")
	EAetherisLOD GlobalLOD = EAetherisLOD::LOD0;

	// Get the chunk ID for a world position
	UFUNCTION(BlueprintCallable, Category = "World")
	int64 GetChunkId(float X, float Z) const;

	// Get chunk coordinates from world position
	UFUNCTION(BlueprintCallable, Category = "World")
	void GetChunkCoords(float X, float Z, int32& ChunkX, int32& ChunkZ) const;

	// World bounding box
	UFUNCTION(BlueprintCallable, Category = "World")
	bool IsInsideWorld(float X, float Z) const;

private:
	float GetChunkOffset(int32 ChunkIdx) const { return ChunkIdx * ChunkSize; }
};

// Aetheris Simulation Manager — core simulation loop and event bus
UCLASS()
class UEATHERISSIMULATION_API UAetherisSimulationManager : public UObject
{
	GENERATED_BODY()

public:
	UAetherisSimulationManager();

	// Called once per simulation frame
	UFUNCTION(BlueprintCallable, Category = "Simulation")
	void TickSimulation(float DeltaTime);

	// Pause/Resume the entire simulation
	UFUNCTION(BlueprintCallable, Category = "Simulation")
	void Pause();

	UFUNCTION(BlueprintCallable, Category = "Simulation")
	void Resume();

	// Set simulation speed (0 = paused, 1 = realtime, 5 = 5x, 20 = 20x, 100 = 100x)
	UFUNCTION(BlueprintCallable, Category = "Simulation")
	void SetSpeedMultiplier(float Speed);

	// Register an entity for simulation (EntityId = 0 → auto-generate from pointer)
	UFUNCTION(BlueprintCallable, Category = "Entity")
	void RegisterEntity(UObject* Entity, const FString& EntityType, uint64 EntityId);

	// Unregister an entity
	UFUNCTION(BlueprintCallable, Category = "Entity")
	void UnregisterEntity(UObject* Entity);

	// Broadcast an event to all listeners
	UFUNCTION(BlueprintCallable, Category = "Event")
	void BroadcastEvent(const EAetherisEventType EventType, const FString& Details = FString());

	// Get simulation time and frame
	UFUNCTION(BlueprintCallable, Category = "Simulation")
	double GetSimulationTime() const { return SimulationTime; }

	UFUNCTION(BlueprintCallable, Category = "Simulation")
	int64 GetSimulationFrame() const { return SimulationFrame; }

private:
	// EntityId → Index map for fast lookup (ADR-002)
	UPROPERTY()
	TMap<uint64, int32> EntityIndexMap;

	// Ordered entity list for iteration
	TArray<UObject*> RegisteredEntities;

	double SimulationTime;
	int64 SimulationFrame;
	float SpeedMultiplier;
	bool bIsPaused;

	// Internal tick for one entity type
	void TickEntityGroup(const FString& EntityType, float DeltaTime);
};

// Aetheris Event Listener interface
UINTERFACE(MinimalAPI)
class UEATHERISSIMULATION_API UAetherisEventListener : public UInterface
{
	GENERATED_BODY()
};

class IAetherisEventListener
{
	GENERATED_BODY()

public:
	virtual void OnAetherisEvent(const EAetherisEventType EventType, const FString& Details) = 0;
};
