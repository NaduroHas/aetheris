// Copyright © 2026 AETHERIS. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AetherisCore.h"
#include "AetherisSimulation.generated.h"

UENUM(BlueprintType)
enum class EAetherisLOD : uint8
{
	LOD0 = 0,
	LOD1 = 1,
	LOD2 = 2,
	LOD3 = 3,
	LOD4 = 4,
};

UENUM(BlueprintType)
enum class EAetherisEventType : uint8
{
	OnNeedsChanged,
	OnStateChange,
	OnDecisionMade,
	OnSkillUsed,
	OnRelationshipChanged,
	OnRelationshipFormed,
	OnRelationshipBroken,
	OnGroupFormed,
	OnGroupDissolved,
	OnResourceSpawned,
	OnResourceDepleted,
	OnWeatherChanged,
	OnSeasonChanged,
	OnTerrainChanged,
	OnTradeOccurred,
	OnProductionOccurred,
	OnConsumptionOccurred,
	OnPriceChanged,
	OnSimulationStep,
	OnDebugEvent,
};

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

UCLASS()
class UEATHERISSIMULATION_API UAetherisWorld : public UObject
{
	GENERATED_BODY()

public:
	UAetherisWorld();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
	float WorldSizeX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
	float WorldSizeZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
	float ChunkSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
	float SimulationSpeed = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Simulation")
	EAetherisLOD GlobalLOD = EAetherisLOD::LOD0;

	UFUNCTION(BlueprintCallable, Category = "World")
	int64 GetChunkId(float X, float Z) const;

	UFUNCTION(BlueprintCallable, Category = "World")
	void GetChunkCoords(float X, float Z, int32& ChunkX, int32& ChunkZ) const;

	UFUNCTION(BlueprintCallable, Category = "World")
	bool IsInsideWorld(float X, float Z) const;
};

UCLASS()
class UEATHERISSIMULATION_API UAetherisSimulationManager : public UObject
{
	GENERATED_BODY()

public:
	UAetherisSimulationManager();

	UFUNCTION(BlueprintCallable, Category = "Simulation")
	void Initialize();

	UFUNCTION(BlueprintCallable, Category = "Simulation")
	void TickSimulation(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Simulation")
	void Pause();

	UFUNCTION(BlueprintCallable, Category = "Simulation")
	void Resume();

	UFUNCTION(BlueprintCallable, Category = "Simulation")
	void SetSpeedMultiplier(float Speed);

	UFUNCTION(BlueprintCallable, Category = "Entity")
	void RegisterEntity(UObject* Entity, const FString& EntityType, uint64 EntityId);

	UFUNCTION(BlueprintCallable, Category = "Entity")
	void UnregisterEntity(UObject* Entity);

	UFUNCTION(BlueprintCallable, Category = "Event")
	void BroadcastEvent(EAetherisEventType EventType, const FString& Details = FString());

	UFUNCTION(BlueprintPure, Category = "Simulation")
	double GetSimulationTime() const { return SimulationTime; }

	UFUNCTION(BlueprintPure, Category = "Simulation")
	int64 GetSimulationFrame() const { return SimulationFrame; }

	UFUNCTION(BlueprintPure, Category = "Simulation")
	float GetSpeedMultiplier() const { return SpeedMultiplier; }

	UFUNCTION(BlueprintPure, Category = "Simulation")
	bool IsPaused() const { return bIsPaused; }

	UFUNCTION(BlueprintCallable, Category = "Simulation")
	void ResetSimulation();

private:
	UPROPERTY()
	TMap<uint64, int32> EntityIndexMap;

	UPROPERTY()
	TArray<TObjectPtr<UObject>> RegisteredEntities;

	double SimulationTime;
	int64 SimulationFrame;
	float SpeedMultiplier;
	bool bIsPaused;
	bool bIsInitialized;
};
