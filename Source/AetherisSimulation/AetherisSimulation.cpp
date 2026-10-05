// Copyright © 2026 AETHERIS. All rights reserved.

#include "AetherisSimulation.h"
#include "AetherisCore.h"

UAetherisSimulationManager::UAetherisSimulationManager()
	: SimulationTime(0.0)
	, SimulationFrame(0)
	, SpeedMultiplier(1.0f)
	, bIsPaused(false)
{
}

void UAetherisSimulationManager::TickSimulation(float DeltaTime)
{
	if (bIsPaused) return;

	// Scale DeltaTime by simulation speed
	float ScaledDelta = DeltaTime * SpeedMultiplier;

	// Update simulation state (single source of truth)
	SimulationTime += ScaledDelta;
	SimulationFrame++;

	// Broadcast simulation step event
	BroadcastEvent(EAetherisEventType::OnSimulationStep,
		FString::Printf(TEXT("Frame %lld, Time %.2f"), SimulationFrame, SimulationTime));

	// Tick all registered entities
	for (auto* Entity : RegisteredEntities)
	{
		if (auto* Tickable = Cast<IAetherisTickable>(Entity))
		{
			Tickable->TickSimulation(ScaledDelta);
		}
	}
}

void UAetherisSimulationManager::Pause()
{
	bIsPaused = true;
	Aetheris::LogCore(TEXT("Simulation paused"));
}

void UAetherisSimulationManager::Resume()
{
	bIsPaused = false;
	Aetheris::LogCore(TEXT("Simulation resumed"));
}

void UAetherisSimulationManager::SetSpeedMultiplier(float Speed)
{
	SpeedMultiplier = FMath::Clamp(Speed, 0.0f, 100.0f);
}

void UAetherisSimulationManager::RegisterEntity(UObject* Entity, const FString& EntityType, uint64 EntityId)
{
	if (!Entity) return;

	// Auto-generate EntityId from pointer if not provided
	if (EntityId == 0)
	{
		EntityId = FMath::HashCombine(
			reinterpret_cast<uint64>(Entity),
			Aetheris::GenerateSeedId(EntityType)
		);
	}

	// ADR-002: EntityId → Index map, not EntityType → Index
	// If EntityId already registered, just ensure it's in RegisteredEntities (idempotent)
	if (!EntityIndexMap.Contains(EntityId))
	{
		EntityIndexMap.Add(EntityId, RegisteredEntities.Num());
		RegisteredEntities.Add(Entity);
	}
}

void UAetherisSimulationManager::UnregisterEntity(UObject* Entity)
{
	if (!Entity) return;

	// Find entity and its index
	int32 Index = INDEX_NONE;
	for (int32 i = 0; i < RegisteredEntities.Num(); ++i)
	{
		if (RegisteredEntities[i] == Entity)
		{
			Index = i;
			break;
		}
	}

	if (Index == INDEX_NONE) return;

	// Find EntityId for this index
	uint64 RemovedId = 0;
	for (auto& Pair : EntityIndexMap)
	{
		if (Pair.Value == Index)
		{
			RemovedId = Pair.Key;
			break;
		}
	}

	// ADR-002: Remove from BOTH structures
	RegisteredEntities.RemoveAt(Index);
	EntityIndexMap.Remove(RemovedId);

	// Update indices for remaining entities
	for (auto& Pair : EntityIndexMap)
	{
		if (Pair.Value > Index)
		{
			Pair.Value--;
		}
	}
}

void UAetherisSimulationManager::BroadcastEvent(const EAetherisEventType EventType, const FString& Details)
{
	FString EventName;
	switch (EventType)
	{
	case EAetherisEventType::OnNeedsChanged: EventName = TEXT("NeedsChanged"); break;
	case EAetherisEventType::OnStateChange: EventName = TEXT("StateChange"); break;
	case EAetherisEventType::OnDecisionMade: EventName = TEXT("DecisionMade"); break;
	case EAetherisEventType::OnSkillUsed: EventName = TEXT("SkillUsed"); break;
	case EAetherisEventType::OnRelationshipChanged: EventName = TEXT("RelationshipChanged"); break;
	case EAetherisEventType::OnRelationshipFormed: EventName = TEXT("RelationshipFormed"); break;
	case EAetherisEventType::OnRelationshipBroken: EventName = TEXT("RelationshipBroken"); break;
	case EAetherisEventType::OnGroupFormed: EventName = TEXT("GroupFormed"); break;
	case EAetherisEventType::OnGroupDissolved: EventName = TEXT("GroupDissolved"); break;
	case EAetherisEventType::OnResourceSpawned: EventName = TEXT("ResourceSpawned"); break;
	case EAetherisEventType::OnResourceDepleted: EventName = TEXT("ResourceDepleted"); break;
	case EAetherisEventType::OnWeatherChanged: EventName = TEXT("WeatherChanged"); break;
	case EAetherisEventType::OnSeasonChanged: EventName = TEXT("SeasonChanged"); break;
	case EAetherisEventType::OnTerrainChanged: EventName = TEXT("TerrainChanged"); break;
	case EAetherisEventType::OnTradeOccurred: EventName = TEXT("TradeOccurred"); break;
	case EAetherisEventType::OnProductionOccurred: EventName = TEXT("ProductionOccurred"); break;
	case EAetherisEventType::OnConsumptionOccurred: EventName = TEXT("ConsumptionOccurred"); break;
	case EAetherisEventType::OnPriceChanged: EventName = TEXT("PriceChanged"); break;
	case EAetherisEventType::OnSimulationStep: EventName = TEXT("SimulationStep"); break;
	case EAetherisEventType::OnDebugEvent: EventName = TEXT("DebugEvent"); break;
	default: EventName = TEXT("UnknownEvent"); break;
	}

	Aetheris::LogCore(FString::Printf(TEXT("[EVENT] %s: %s"), *EventName, *Details));

	// Notify all listeners
	for (auto* Entity : RegisteredEntities)
	{
		if (auto* Listener = Cast<IAetherisEventListener>(Entity))
		{
			Listener->OnAetherisEvent(EventType, Details);
		}
	}
}

int64 UAetherisWorld::GetChunkId(float X, float Z) const
{
	int32 ChunkX, ChunkZ;
	GetChunkCoords(X, Z, ChunkX, ChunkZ);
	return static_cast<int64>(ChunkX) * 1000000LL + static_cast<int64>(ChunkZ);
}

void UAetherisWorld::GetChunkCoords(float X, float Z, int32& ChunkX, int32& ChunkZ) const
{
	ChunkX = FMath::FloorToInt((X + WorldSizeX * 0.5f) / ChunkSize);
	ChunkZ = FMath::FloorToInt((Z + WorldSizeZ * 0.5f) / ChunkSize);
}

bool UAetherisWorld::IsInsideWorld(float X, float Z) const
{
	return X >= -WorldSizeX * 0.5f && X <= WorldSizeX * 0.5f &&
		   Z >= -WorldSizeZ * 0.5f && Z <= WorldSizeZ * 0.5f;
}
