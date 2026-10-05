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

	// Update global simulation state
	Aetheris::SimulationTime += ScaledDelta;
	Aetheris::SimulationFrame++;

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

void UAetherisSimulationManager::RegisterEntity(UObject* Entity, const FString& EntityType)
{
	if (Entity && !EntityIndexMap.Contains(EntityType))
	{
		EntityIndexMap.Add(EntityType, EntityIndexMap.Num());
	}
	RegisteredEntities.Add(Entity);
}

void UAetherisSimulationManager::UnregisterEntity(UObject* Entity)
{
	RegisteredEntities.Remove(Entity);
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
