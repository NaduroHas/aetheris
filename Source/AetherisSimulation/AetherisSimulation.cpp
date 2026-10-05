// Copyright © 2026 AETHERIS. All rights reserved.

#include "AetherisSimulation.h"

UAetherisWorld::UAetherisWorld()
	: WorldSizeX(2000.f)
	, WorldSizeZ(2000.f)
	, ChunkSize(500.f)
{
}

UAetherisIndividual::UAetherisIndividual()
	: UnitId(0)
	, Species(EAetherisSpecies::Human)
	, Sex(EAetherisSex::Male)
	, AgeYears(0.0)
	, Hunger(0.0f)
	, Thirst(0.0f)
	, SocialNeed(0.0f)
	, Fatigue(0.0f)
	, Health(100.0f)
	, CurrentLOD(EAetherisLOD::LOD0)
{
}

void UAetherisIndividual::Initialize(uint64 InUnitId, const FString& InName, EAetherisSpecies InSpecies, EAetherisSex InSex)
{
	if (InUnitId == 0)
	{
		Aetheris::LogError(TEXT("[Individual] Initialize rejected: UnitId must be stable and non-zero."));
		return;
	}

	UnitId = InUnitId;
	DisplayName = InName;
	Species = InSpecies;
	Sex = InSex;
	AgeYears = 0.0;
	Health = 100.0f;
	CurrentLOD = EAetherisLOD::LOD0;
	ResetNeeds();
}

void UAetherisIndividual::ResetNeeds()
{
	Hunger = 0.0f;
	Thirst = 0.0f;
	SocialNeed = 0.0f;
	Fatigue = 0.0f;
}

float UAetherisIndividual::GetCriticalNeed() const
{
	return FMath::Max(FMath::Max(Hunger, Thirst), FMath::Max(SocialNeed, Fatigue));
}

void UAetherisIndividual::TickSimulation(float DeltaTime)
{
	if (!IsAlive() || DeltaTime <= 0.0f)
	{
		return;
	}

	// Balancing rates are intentionally not hard-coded here. Dedicated systems
	// will own needs, ageing and actions.
	(void)DeltaTime;
}

UAetherisSimulationManager::UAetherisSimulationManager()
	: SimulationTime(0.0)
	, SimulationFrame(0)
	, SpeedMultiplier(1.0f)
	, bIsPaused(false)
	, bIsInitialized(false)
{
}

void UAetherisSimulationManager::Initialize()
{
	ResetSimulation();
	bIsInitialized = true;
	Aetheris::LogCore(TEXT("[Simulation] Initialized"));
}

void UAetherisSimulationManager::ResetSimulation()
{
	SimulationTime = 0.0;
	SimulationFrame = 0;
	SpeedMultiplier = 1.0f;
	bIsPaused = false;

	for (TPair<uint64, double>& Pair : EntityNextTickTime)
	{
		Pair.Value = 0.0;
	}
}

void UAetherisSimulationManager::TickSimulation(float DeltaTime)
{
	if (!bIsInitialized || bIsPaused || DeltaTime <= 0.0f)
	{
		return;
	}

	const double ScaledDelta = static_cast<double>(DeltaTime) * static_cast<double>(SpeedMultiplier);
	SimulationTime += ScaledDelta;
	++SimulationFrame;

	BroadcastEvent(
		EAetherisEventType::OnSimulationStep,
		FString::Printf(TEXT("Frame %lld, Time %.2f"), SimulationFrame, SimulationTime));

	for (TObjectPtr<UObject>& Entity : RegisteredEntities)
	{
		if (!IsValid(Entity))
		{
			continue;
		}

		const int32 EntityIndex = &Entity - RegisteredEntities.GetData();
		if (!RegisteredEntityIds.IsValidIndex(EntityIndex))
		{
			continue;
		}

		const uint64 EntityId = RegisteredEntityIds[EntityIndex];
		const double TickInterval = EntityTickIntervals.FindRef(EntityId);
		double& NextTickTime = EntityNextTickTime.FindOrAdd(EntityId, SimulationTime);

		if (TickInterval > 0.0 && SimulationTime + KINDA_SMALL_NUMBER < NextTickTime)
		{
			continue;
		}

		if (IAetherisTickable* Tickable = Cast<IAetherisTickable>(Entity.Get()))
		{
			Tickable->TickSimulation(static_cast<float>(ScaledDelta));
		}

		NextTickTime = SimulationTime + TickInterval;
	}
}

void UAetherisSimulationManager::Pause()
{
	bIsPaused = true;
	Aetheris::LogCore(TEXT("[Simulation] Paused"));
}

void UAetherisSimulationManager::Resume()
{
	bIsPaused = false;
	Aetheris::LogCore(TEXT("[Simulation] Resumed"));
}

void UAetherisSimulationManager::SetSpeedMultiplier(float Speed)
{
	SpeedMultiplier = FMath::Clamp(Speed, 0.0f, 100.0f);
}

UAetherisIndividual* UAetherisSimulationManager::CreateIndividual(const FString& Name, EAetherisSpecies Species, EAetherisSex Sex, uint64 UnitId)
{
	if (!bIsInitialized || UnitId == 0 || EntityIndexMap.Contains(UnitId))
	{
		Aetheris::LogError(TEXT("[Simulation] CreateIndividual rejected: invalid state or duplicate UnitId."));
		return nullptr;
	}

	UAetherisIndividual* Individual = NewObject<UAetherisIndividual>(this);
	Individual->Initialize(UnitId, Name, Species, Sex);
	RegisterEntity(Individual, TEXT("Individual"), UnitId);
	return Individual;
}

void UAetherisSimulationManager::RegisterEntity(UObject* Entity, const FString& EntityType, uint64 EntityId)
{
	if (!IsValid(Entity))
	{
		return;
	}

	if (EntityId == 0)
	{
		Aetheris::LogError(TEXT("[Simulation] RegisterEntity rejected: EntityId must be stable and non-zero."));
		return;
	}

	if (EntityIndexMap.Contains(EntityId))
	{
		Aetheris::LogWarning(FString::Printf(
			TEXT("[Simulation] EntityId %llu already registered; ignoring duplicate."),
			EntityId));
		return;
	}

	const int32 NewIndex = RegisteredEntities.Num();
	EntityIndexMap.Add(EntityId, NewIndex);
	EntityTickIntervals.Add(EntityId, 0.0);
	EntityNextTickTime.Add(EntityId, SimulationTime);
	RegisteredEntityIds.Add(EntityId);
	RegisteredEntities.Add(Entity);

	Aetheris::LogCore(FString::Printf(
		TEXT("[Simulation] Registered entity %s (%llu). Total=%d"),
		*EntityType, EntityId, RegisteredEntities.Num()));
}

void UAetherisSimulationManager::UnregisterEntity(UObject* Entity)
{
	if (!IsValid(Entity))
	{
		return;
	}

	int32 RemovedIndex = INDEX_NONE;
	for (int32 Index = 0; Index < RegisteredEntities.Num(); ++Index)
	{
		if (RegisteredEntities[Index].Get() == Entity)
		{
			RemovedIndex = Index;
			break;
		}
	}

	if (RemovedIndex == INDEX_NONE)
	{
		return;
	}

	uint64 RemovedId = 0;
	for (const TPair<uint64, int32>& Pair : EntityIndexMap)
	{
		if (Pair.Value == RemovedIndex)
		{
			RemovedId = Pair.Key;
			break;
		}
	}

	RegisteredEntities.RemoveAt(RemovedIndex);
	RegisteredEntityIds.RemoveAt(RemovedIndex);
	if (RemovedId != 0)
	{
		EntityIndexMap.Remove(RemovedId);
		EntityTickIntervals.Remove(RemovedId);
		EntityNextTickTime.Remove(RemovedId);
	}

	for (TPair<uint64, int32>& Pair : EntityIndexMap)
	{
		if (Pair.Value > RemovedIndex)
		{
			--Pair.Value;
		}
	}
}

void UAetherisSimulationManager::SetEntityTickInterval(uint64 EntityId, double TickIntervalSeconds)
{
	if (!EntityIndexMap.Contains(EntityId))
	{
		Aetheris::LogWarning(FString::Printf(
			TEXT("[Scheduler] Cannot configure unknown EntityId %llu."),
			EntityId));
		return;
	}

	const double ClampedInterval = FMath::Max(0.0, TickIntervalSeconds);
	EntityTickIntervals.FindOrAdd(EntityId) = ClampedInterval;
	EntityNextTickTime.FindOrAdd(EntityId) = SimulationTime + ClampedInterval;
}

double UAetherisSimulationManager::GetEntityTickInterval(uint64 EntityId) const
{
	if (!EntityIndexMap.Contains(EntityId))
	{
		return 0.0;
	}

	const double* Interval = EntityTickIntervals.Find(EntityId);
	return Interval != nullptr ? *Interval : 0.0;
}

void UAetherisSimulationManager::BroadcastEvent(EAetherisEventType EventType, const FString& Details)
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

	for (TObjectPtr<UObject>& Entity : RegisteredEntities)
	{
		if (!IsValid(Entity))
		{
			continue;
		}

		if (IAetherisEventListener* Listener = Cast<IAetherisEventListener>(Entity.Get()))
		{
			Listener->OnAetherisEvent(EventType, Details);
		}
	}
}

int64 UAetherisWorld::GetChunkId(float X, float Z) const
{
	int32 ChunkX = 0;
	int32 ChunkZ = 0;
	GetChunkCoords(X, Z, ChunkX, ChunkZ);
	return (static_cast<int64>(ChunkX) << 32) ^ static_cast<uint32>(ChunkZ);
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