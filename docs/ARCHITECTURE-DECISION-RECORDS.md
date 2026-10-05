---
title: "AETHERIS Architecture Decision Records"
date: "2026-10-05"
author: "der-ingenieur"
version: "0.1"
---

# AETHERIS — Architecture Decision Records (ADRs)

## ADR-001: Single Source of Truth für Simulation State

**Status**: PROPOSED
**Date**: 2026-10-05

### Context
Aktuell gibt es DUPLICATE state für SimulationTime und SimulationFrame:
- `Aetheris::SimulationTime` (namespace static in AetherisCore.cpp Zeile 24)
- `UAetherisSimulationManager::SimulationTime` (Member in AetherisSimulation.h Zeile 164)

### Problem
`TickSimulation()` (AetherisSimulation.cpp Zeile 22-23) aktualisiert nur die NAMESPACE-Variablen:
```cpp
Aetheris::SimulationTime += ScaledDelta;  // Namespace ↑
Aetheris::SimulationFrame++;              // Namespace ↑
```

Aber `BroadcastEvent()` (Zeile 26-27) liest die MEMBER-Variablen:
```cpp
FString::Printf(TEXT("Frame %lld, Time %.2f"), SimulationFrame, SimulationTime)
```

Da `this->SimulationTime` im Konstruktor auf 0 initialisiert wird und NIE inkrementiert wird,
broadcastet das Event-System immer "Frame 0, Time 0.00".

`Aetheris::GetSimulationTime()` (Core.cpp Zeile 55-57) liest den NAMESPACE-Wert.
`Manager->GetSimulationTime()` (Sim.h Zeile 155) liest den MEMBER-Wert → DIVERGENT.

### Decision
**Einzige Quelle: UAetherisSimulationManager.**
- Namespace-Variablen `Aetheris::SimulationTime` und `Aetheris::SimulationFrame` REMOVEN
- `Aetheris::GetSimulationTime()` und `Aetheris::GetSimulationFrame()` werden DEPRECATED
- Alle Time/Frame-Zugriffe laufen über `Manager->GetSimulationTime()` und `GetSimulationFrame()`
- `TickSimulation()` updatet nur die Member-Variablen (bisher korrekt, nur Event nutzt stale member)
- StartupModule-Initialisierung (Core.cpp Zeile 12-13) wird entfernt

### Alternatives Considered
- **Hybrid-Ansatz**: Namespace für globalen Zustand, Member für lokale. Vorteil: Flexibel für Multi-Simulation. Nachteil: Komplexität, Fehleranfälligkeit. Für Phase 1 überflüssig.
- **Core-Only**: Zeit nur in Core. Nachteil: SimulationManager kann nicht unabhängig ticken, keine Save/Load mit separaten States.

### Consequences
- Breaking Change: Alle Module, die `Aetheris::GetSimulationTime()` aufrufen, müssen umgeschrieben werden
- Vereinfacht die Architektur: Ein Objekt, eine Wahrheit
- Enables future: Multiple SimulationManager für Save/Load oder Multi-World

---

## ADR-002: EntityIndexMap — EntityId-basiertes Mapping

**Status**: PROPOSED
**Date**: 2026-10-05

### Context
`RegisterEntity()` (Sim.cpp Zeile 56-63) mappt `EntityType (FString) → Index (int32)`, nicht Entity → Index.

### Problem
1. Zwei Entities mit gleichem Typ (z.B. "Unit") überschreiben denselben Index in EntityIndexMap
2. `UnregisterEntity()` (Sim.cpp Zeile 65-68) entfernt Entity aus RegisteredEntities, ABER NICHT aus EntityIndexMap → Memory Leak im TMap
3. Es gibt keine Möglichkeit, eine Entity ID-basiert zu lookupen

### Decision
- `EntityIndexMap` umwandeln von `TMap<FString, int32>` zu `TMap<uint64, int32>` (EntityId → Index)
- `RegisterEntity()` nimmt zusätzlich `uint64 EntityId` als Parameter
- `UnregisterEntity()` entfernt auch aus EntityIndexMap
- Optional: `TMultiMap<uint64, FString>` für EntityId → EntityType (umgekehrte Lookup)

### Alternatives Considered
- **TMap<EntityPtr, EntityId>**: Speichert Pointer statt Index. Problem: Pointer können invalid werden (GC). EntityId ist stabil.
- **FEntity Registry als separates UObject**: Mehr Overhead, aber besser trennbar. Für Phase 1 zu viel.

---

## ADR-003: Game-Loop-Architektur

**Status**: PROPOSED
**Date**: 2026-10-05

### Context
`UAetherisSimulationManager::TickSimulation()` existiert (Sim.cpp Zeile 14-37), wird aber NIEMAND aufgerufen.
GameMode hat keinen `BeginPlay()`-Override.

### Decision
- `AAetherisGameModeBase::BeginPlay()` aufrufen: `Super::BeginPlay(); SimulationManager->Initialize();`
- SimulationManager tickt via `UWorld::OnWorldTick` delegate ODER GameMode tickt selbst
- Präferenz: GameMode tickt und leitet an SimulationManager weiter (einfacher, keine Delegate-Abhängigkeit)

```cpp
void AAetherisGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    if (SimulationManager)
    {
        SetTickable(true);
    }
}

void AAetherisGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (SimulationManager)
    {
        SimulationManager->TickSimulation(DeltaTime);
    }
}
```

### Consequences
- GameMode muss `bAutoManageActiveGameplay` korrekt setzen
- Wenn Simulation pausiert ist, tickt GameMode weiterhin (für Rendering/Input), aber SimulationManager returnt sofort
- Future: Kann durch UWorld::OnWorldTick ersetzt werden, wenn mehr Flexibilität nötig ist
