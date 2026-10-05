# KI-Verhaltens-Spec — Phase 1: Individual System

**Autor:** @der-beobachter  
**Status:** Entwurf → Review nötig  
**Priorität:** Phase 0.5 Parallel-Task (nach Dirigenten-Plan)  
**Abhängigkeiten:** ADR-001 (Single Source of Truth), ADR-002 (EntityId-Mapping)

---

## 1. ÜBERBLICK

Phase 1 definiert das Verhalten **eines einzelnen autonomen Individuums**. Keine Schwärme, keine Multiplayer-Interaktion. Ein Entity mit:

- **Bedürfnissen** (Hunger, Energie, Soziales)
- **Lokaler Wahrnehmung** (sichtbare Entities, Ressourcen, Gefahren)
- **Ziel-getriebenem Entscheidungsverhalten** (Utility AI)
- **Erinnerung** (vergessen/gelernt)

### KI-Prinzipien (unverhandelbar)

| Prinzip | Bedeutung |
|---|---|
| **Lokale Wahrnehmung** | Kein "God View". Entity sieht nur was in `PerceptionRadius`. |
| **Needs-Driven** | Verhalten wird getrieben durch unbefriedigte Bedürfnisse, nicht State-Machine. |
| **Simple Rules, Complex Behavior** | Maximale 5 Kernregeln pro Entity. Emergenz entsteht, nicht wird programmiert. |
| **Memory > Brute Force** | Entities erinnern sich: Wer war freundlich? Wo war Wasser? Was war gefährlich? |
| **Performance Budget** | Pro Entity pro Tick ≤ **0.05ms** (Single-Core, 200 Entities = 10ms Total). |

---

## 2. NEEDS-SYSTEM

### 2.1 Bedürfnisse-Definition

Jedes Entity hat drei primitive Bedürfnisse mit Werten 0.0–1.0:

| Need | Reichweite | Sink-Rate | Kritisch bei | Beschreibung |
|---|---|---|---|---|
| **Hunger** | 0.0–1.0 | 0.002/Tick (Basis) | < 0.2 | Nahrungsbedarf. Unter 0.2 → Panik-Verhalten. Unter 0.0 → Sterben. |
| **Energie** | 0.0–1.0 | 0.003/Tick (aktiv), 0.001/Tick (Ruhe) | < 0.15 | Arbeits- und Bewegungskapazität. Unter 0.15 → Zwangsschlaf. Unter 0.0 → Bewusstlos. |
| **Soziales** | 0.0–1.0 | 0.001/Tick (isoliert), 0.0003/Tick (in Gruppe) | < 0.2 | Bindungsbedarf. Unter 0.2 → Aggressivität/Gier. Unter 0.0 → Depression (Inaktivität). |

**Design-Entscheidung:** Drei Bedürfnisse, nicht mehr. Das ist die minimale Set für emergentes Verhalten (Survival + Rest + Connection — Maslow Lite). Mehr Bedürfnisse = mehr Kombinatorik, aber keine zusätzliche Emergenz in Phase 1.

### 2.2 Need-Druck (Need Pressure)

Der **Need-Druck** bestimmt die Priorität des Verhaltens:

```
NeedPressure = Σ(NeedWeight[i] × (1.0 - CurrentNeed[i]))

Wobei NeedWeight basierend auf Kritikalität:
  Hunger:   1.0 (überlebenskritisch)
  Energie:  0.8 (überlebenskritisch)
  Soziales: 0.5 (nicht-tödlich)
```

**Beispiel:**
- Hunger = 0.1, Energie = 0.8, Soziales = 0.7 → Pressure = 0.9×1.0 + 0.2×0.8 + 0.3×0.5 = **1.01**
- Hunger = 0.7, Energie = 0.7, Soziales = 0.3 → Pressure = 0.3×1.0 + 0.3×0.8 + 0.7×0.5 = **0.89**

→ Entity mit niedrigem Hunger und niedrigem Soziales ist **weniger getrieben** als eines mit kritischem Hunger. Das ist der **primäre Treiber** für jede Entscheidungsfindung.

### 2.3 Need-Implementation

```cpp
// UAetherisNeedsComponent (neues Component)
UCLASS()
class UEATHERISSIMULATION_API UAetherisNeedsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Needs")
    FNeedDefinition HungerDef;
    
    UPROPERTY(EditDefaultsOnly, Category = "Needs")
    FNeedDefinition EnergyDef;
    
    UPROPERTY(EditDefaultsOnly, Category = "Needs")
    FNeedDefinition SocialDef;

    // Update all needs (called per tick)
    void TickNeeds(float DeltaTime);

    // Get highest pressure need
    EAetherisNeed GetMostPressingNeed() const;

    // Get weighted need pressure (0.0 = calm, 1.0 = desperate)
    float GetOverallPressure() const;

private:
    float CurrentHunger;
    float CurrentEnergy;
    float CurrentSocial;

    // Need definition: sink rate, critical threshold, weight
    USTRUCT()
    struct FNeedDefinition
    {
        GENERATED_BODY()
        UPROPERTY(EditDefaultsOnly) float SinkRatePerSecond = 0.001f;
        UPROPERTY(EditDefaultsOnly) float CriticalThreshold = 0.2f;
        UPROPERTY(EditDefaultsOnly) float PressureWeight = 0.5f;
        UPROPERTY(EditDefaultsOnly) float MaxValue = 1.0f;
    };
};
```

**Wichtig:** Alle Need-Werte und Sink-Rates sind **konfigurierbar über DataAssets**. Keine hardcoded floats im Code. Das erlaubt Balancing ohne Code-Change.

---

## 3. PERZEPTION-SYSTEM

### 3.1 Prinzip: Lokale Wahrnehmung

**EINZIGE Regel:** Ein Entity kann NICHT sehen, was außerhalb seines `PerceptionRadius` liegt.

```
PerceptionRadius: 150.0 units (Standard, konfigurierbar)
PerceptionCone: 360° (omnidirektional, später optional konisch)
```

### 3.2 Wahrnehmbare Objekte

| Kategorie | Wie gefunden | Was wird übertragen |
|---|---|---|
| **Andere Entities** | Overlap-Test im Chunk + Distance-Filter | ID, Distance, Direction, CurrentState, CurrentNeed-Druck (gerundet) |
| **Ressourcen** (Wasser, Nahrung) | Overlap-Test im Chunk | ID, Distance, Resource-Type, Menge |
| **Gefahren** (Fire, Predator) | Proximity-Event (Event-System) | Distance, Severity, SourceID |
| **Wetter/Season** | Event-System (global, von SimulationManager) | EventType, Details |

### 3.3 Perzeptions-Implementation

```cpp
// UAetherisPerceptionComponent
UCLASS()
class UEATHERISSIMULATION_API UAetherisPerceptionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Called when nearby entities/resources change
    void Reperceive();

    // Get all visible entities within radius
    TArray<PerceivedEntity> GetVisibleEntities() const;
    
    // Get nearest resource of type
    PerceivedResource* GetNearestResource(EAetherisResourceType Type) const;

    // Get perceived threats
    TArray<PerceivedThreat> GetPerceivedThreats() const;

private:
    float PerceptionRadius = 150.0f;
    
    // Cache of last known state of nearby entities (reduces tick-cost)
    TMap<uint64, PerceivedEntity> LastKnownEntities;
    
    // Only store direction + state, not full entity ptr (loose coupling)
    USTRUCT()
    struct PerceivedEntity
    {
        uint64 EntityId;
        float Distance;
        FVector Direction;
        int8 StateApproximation;  // Enum as byte, not full enum
        int8 PressureApproximation;  // Quantized 0-10
    };
};
```

**Performance-Optimierung:**  
- Perzeption wird **nicht pro Tick** neu berechnet.  
- Sondern: **Event-gesteuert**. Wenn sich ein Entity bewegt (> Delta), feuert es ein `OnMoved` Event. Perzeption-Component hört mit → aktualisiert Cache.  
- Reduziert per-Etikett-Kosten von O(N) pro Tick auf O(ΔN) pro Tick (nur Änderungen).

---

## 4. ENTSCHEIDUNGS-SYSTEM

### 4.1 Utility AI — Das Herzstück

Jedes Entity bewertet **jeden verfügbaren Aktion** bei jedem Entscheidungs-Zyklus und wählt die mit höchster Utility.

```
Utility(Action) = Σ(Weight[Need] × Effect[Action, Need])

Beispiel: Entity hat Hunger=0.1, Energy=0.6, Social=0.7
  Action "Eat" → Hunger +0.4, Energy -0.05, Social +0.0
    Utility = 1.0 × 0.4 + 0.8 × (-0.05) + 0.5 × 0.0 = 0.36
  
  Action "Sleep" → Hunger -0.05, Energy +0.5, Social +0.0
    Utility = 1.0 × (-0.05) + 0.8 × 0.5 + 0.5 × 0.0 = 0.375
  
  Action "Socialize" → Hunger -0.02, Energy -0.03, Social +0.3
    Utility = 1.0 × (-0.02) + 0.8 × (-0.03) + 0.5 × 0.3 = 0.097

→ "Sleep" gewinnt mit 0.375
```

### 4.2 Verfügbarkeit von Aktionen

Nicht alle Aktionen sind immer verfügbar. Filter:

| Aktion | Verfügbar wenn | Kosten |
|---|---|---|
| **Eat** | Ressource "Food" in Perzeption | -0.05 Energy |
| **Drink** | Ressource "Water" in Perzeption | -0.02 Energy |
| **Sleep** | Energie < 0.5, keine Gefahr in 50 units | -1 Tick (Energy +0.5) |
| **Rest** | Keine Action verfügbar | -0.01 Energy (Erholung) |
| **Explore** | Hunger > 0.5, keine dringende Need | -0.02 Energy |
| **Socialize** | Mindestens ein anderes Entity in Perzeption, Social < 0.6 | -0.03 Energy |
| **Flee** | Gefahr in Perzeption | -0.05 Energy (Bewegung) |

### 4.3 Entscheidungs-Zyklus

```
┌─────────────────────────────────────┐
│              TICK                    │
│                                      │
│  1. Needs.TickNeeds(DeltaTime)      │  ← Bedarf aktualisieren
│  2. Perception.Reperceive()         │  ← Umgebung scannen
│  3. Memory.Update()                 │  ← Erinnerungen updaten
│  4. DecisionCycle()                 │  ← Utility AI
│     ├─ GetAvailableActions()        │  ← Welche Actions sind möglich?
│     ├─ EvaluateUtility(Actions)     │  ← Utility berechnen
│     ├─ SelectBestAction()           │  ← Höchste Utility
│     └─ ExecuteAction(Selected)      │  ← Aktion starten
│  5. BroadcastEvent(OnDecisionMade)  │  ← Event für Logging
└─────────────────────────────────────┘
```

**WICHTIG:** Die Evaluation findet **alle N Ticks** statt, nicht jeden Tick.  
`DecisionInterval = max(1, int(1.0 / OverallPressure))` — bei hohem Druck (Hunger < 0.2) → alle 1 Tick. Bei kalmen Bedingungen → alle 5–10 Ticks. Das spart CPU.

### 4.4 Utility AI Implementation

```cpp
// UAetherisDecisionComponent
UCLASS()
class UEATHERISSIMULATION_API UAetherisDecisionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    void TickDecision(float DeltaTime);
    
    void EvaluateAndExecute();

private:
    // Available actions for this entity
    TArray<UAetherisAction*> AvailableActions;
    
    // Current decision timestamp
    float LastDecisionTime;
    float DecisionInterval = 5.0f;

    // Evaluate all available actions, return best
    UAetherisAction* SelectBestAction();
    
    // Execute action over time (may take multiple ticks)
    void ExecuteAction(UAetherisAction* Action);
    
    // Utility weights from DataAsset
    UPROPERTY(EditDefaultsOnly, Category = "Utility")
    TSoftObjectPtr<UDataTable> UtilityWeightsTable;
};
```

**Utility-Gewichte** liegen in einem **DataTable** (Blueprint DataAsset), nicht im Code. Das ist **konfigurierbar vom Game Designer** ohne Code-Change.

---

## 5. ERINNERUNGS-SYSTEM

### 5.1 Memory als Verhaltens-Treiber

Entities erinnern sich:

| Memory-Type | Lebensdauer | Inhalt | Verhalten-Output |
|---|---|---|---|
| **Location Memory** | Permanent | Wo war X? (Nahrung, Wasser, Gefahren) | Navigation zu bekannten Orten |
| **Social Memory** | 30 Tage (Sim) | Entity Y war freundlich/aggressiv | Socializing-Bias |
| **Event Memory** | 7 Tage | Was ist passiert? (Sturm, Feuer) | Reaktiv auf ähnliche Events |
| **State Memory** | Bis Reset | Aktuelles Ziel ("Ich esse gerade") | State-Machine Overlay |

### 5.2 Memory-Implementation

```cpp
// UAetherisMemoryComponent
UCLASS()
class UEATHERISSIMULATION_API UAetherisMemoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Remember an entity interaction
    void RememberSocialInteraction(uint64 TargetId, EMemoryValence Valence);
    
    // Get social valence for entity
    EMemoryValence GetSocialValence(uint64 TargetId) const;
    
    // Store a location event
    void RememberLocation(float X, float Z, EAetherisResourceType Resource);
    
    // Get nearest remembered resource of type
    FVector GetNearestRememberedLocation(EAetherisResourceType Type) const;
    
    // Expire old memories
    void ExpireMemories(double CurrentTime);

private:
    USTRUCT()
    struct FSocialMemory
    {
        uint64 TargetId;
        EMemoryValence Valence;  // Positive, Negative, Neutral
        double LastInteraction;
        int8 Strength;  // 0-10, wie stark die Erinnerung
    };
    
    USTRUCT()
    struct FLocationMemory
    {
        FVector Location;
        EAetherisResourceType Resource;
        double Timestamp;
        int8 Reliability;  // 0-10, wie zuverlässig (1 = einmal gesehen, 10 = 5x bestätigt)
    };

    TArray<FSocialMemory> SocialMemories;
    TArray<FLocationMemory> LocationMemories;
};
```

---

## 6. ACTION-DEFINITIONEN

Jede Action ist ein **UClass**, der von `UAetherisAction` erbt. Das erlaubt **Data-Driven Action-Erweiterung** ohne Code-Change für neue Actions.

```cpp
// Base action class
UCLASS(Abstract)
class UEATHERISSIMULATION_API UAetherisAction : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent)
    float GetUtility() const;  // Utility Score (0.0–1.0)
    
    UFUNCTION(BlueprintNativeEvent)
    bool IsAvailable() const;  // Ist diese Action gerade möglich?
    
    UFUNCTION(BlueprintNativeEvent)
    void Execute();  // Führe die Aktion aus
    
    UFUNCTION(BlueprintNativeEvent)
    bool IsExecuting();  // Läuft diese Aktion?
    
    UFUNCTION(BlueprintNativeEvent)
    void Tick(float DeltaTime);  // Pro-Tick Updates während der Ausführung
};
```

### Phase 1 Actions (implementiert)

| Action | Class Name | Beschreibung |
|---|---|---|
| Eat | `UAetherisActionEat` | Finde Nahrung, bewege dich hin, esse |
| Drink | `UAetherisActionDrink` | Finde Wasser, bewege dich hin, trinke |
| Sleep | `UAetherisActionSleep` | Finde sicheren Ort, schlafe |
| Rest | `UAetherisActionRest` | Bleibe stehen, erhole Energy |
| Explore | `UAetherisActionExplore` | Bewege dich in zufällige Richtung, scanne Umwelt |
| Socialize | `UAetherisActionSocialize` | Finde nächstes Entity, initiere Interaction |
| Flee | `UAetherisActionFlee` | Bewege dich weg von Gefahr |

### Action-Feederung (Stacking)

Actions werden **nicht** einfach überschrieben. Ein Active Action Stack:

```
ActiveAction: Eat (Duration: 10 Ticks, 5 remaining)
PendingAction: Sleep (wird aktiv wenn Eat abgeschlossen)
```

Wenn eine **höhere Utility Action** verfügbar wird (z.B. Gefahr → Flee), wird die **aktuelle Action abgebrochen** und die neue ausgeführt. Das ist der **Interrupt-Mechanismus**.

```cpp
bool CanInterrupt(UAetherisAction* Current, UAetherisAction* New) const
{
    if (New->GetPriority() >= EActionPriority::High) return true;
    if (Current->IsInterruptible()) return true;
    return false;
}
```

### Action Priority Levels

| Priority | Actions | Unterbrechbar |
|---|---|---|
| **Critical** | Flee (Gefahr) | Immer |
| **High** | Eat, Drink | Nein (außer Critical) |
| **Medium** | Sleep, Rest | Ja |
| **Low** | Explore, Socialize | Ja |

---

## 7. LOCOMOTION (Bewegung)

### 7.1 Simple Movement

Phase 1: **Ziel-basierte Bewegung**. Kein NavMesh (noch nicht verfügbar).

```
Bewegungs-Algorithmus:
  1. Ziel-Position berechnen (von Action)
  2. Direkte Line berechnen
  3. Bewegung in Richtung Ziel mit MaxSpeed
  4. Kollisionserkennung: wenn Blockierung → Stop + Wait
```

**Warum nicht NavMesh?** @der-weltenbauer muss erst NavMeshes definieren. Phase 1 tut mit einfachem Line-Movement + Kollisions-Check. Später wird das durch NavMesh ersetzt.

### 7.2 Bewegungsgeschwindigkeit

| Zustand | Speed (units/sec) |
|---|---|
| Exploring | 30.0 |
| Eating | 0.0 (idle) |
| Drinking | 0.0 (idle) |
| Sleeping | 0.0 (idle) |
| Fleeing | 60.0 |
| Socializing | 20.0 |
| Resting | 0.0 (idle) |

**Speed-Multiplier basierend auf Energy:**  
`EffectiveSpeed = BaseSpeed × (Energy / MaxEnergy)`  
→ Energiedefizit verlangsamt physische Bewegung, aber nicht die Entscheidungsfindung.

---

## 8. INTEGRATION MIT EXISTIERENDEN SYSTEMEN

### 8.1 IAetherisTickable

```cpp
// Entity muss IAetherisTickable implementieren
void UAetherisEntity::TickSimulation(float DeltaTime)
{
    NeedsComponent->TickNeeds(DeltaTime);
    DecisionComponent->TickDecision(DeltaTime);
    // Movement und andere Updates
}
```

### 8.2 IAetherisEventListener

```cpp
// Entity muss IAetherisEventListener implementieren
void UAetherisEntity::OnAetherisEvent(EAetherisEventType EventType, const FString& Details)
{
    switch (EventType)
    {
        case EAetherisEventType::OnNeedsChanged:
            // Re-evaluate decision
            DecisionComponent->MarkDecisionStale();
            break;
        case EAetherisEventType::OnWeatherChanged:
            Memory->RememberWeatherEvent(Details);
            if (Details.Contains("Storm")) AvailableActions->EnableInterrupt(EActionPriority::Critical);
            break;
        case EAetherisEventType::OnSimulationStep:
            // Frame-Counter für Timing (z.B. Sleep nach N Ticks)
            SimulationFrameReceived = SimulationManager->GetSimulationFrame();
            break;
        default:
            // Default: keine Aktion
            break;
    }
}
```

### 8.3 ADR-002 Kompatibilität

Nach ADR-002 (EntityId-basiertes Mapping):

```cpp
// RegisterEntity muss EntityId akzeptieren
void UAetherisSimulationManager::RegisterEntity(uint64 EntityId, const FString& EntityType);

// Memory verwendet EntityId, nicht Pointer
void RememberSocialInteraction(uint64 TargetId, EMemoryValence Valence);
```

---

## 9. PERFORMANCE BUDGET

### 9.1 Pro Entity (Phase 1)

| Komponente | Zeit/Tick | Budget |
|---|---|---|
| Needs.Update | 0.003ms | ≤ 0.01ms |
| Perception.Reperceive | 0.005ms (event-geprüft, nur ΔN) | ≤ 0.02ms |
| Decision.Evaluate | 0.025ms (nur alle N Ticks) | ≤ 0.05ms |
| Memory.Update | 0.002ms (nur Expire bei Bedarf) | ≤ 0.01ms |
| Movement.Tick | 0.015ms | ≤ 0.02ms |
| **Total per Entity** | **~0.05ms** | **≤ 0.1ms** |

### 9.2 Total System (200 Entities)

```
200 × 0.05ms = 10ms pro Tick
10ms / 60 FPS = 0.17% CPU-Verbrauch
```

### 9.3 LOD-System Integration

| LOD | Decision Interval | Actions Available | Perception |
|---|---|---|---|
| **LOD0** (Full) | Jeder Tick (bei Druck) | Alle | Vollständig (Radius 150) |
| **LOD1** (Medium) | Alle 2 Ticks | Alle außer Explore | Radius 100 |
| **LOD2** (Low) | Alle 5 Ticks | Eat, Drink, Flee | Radius 50 |
| **LOD3** (Mass) | Alle 10 Ticks | Nur Eat, Flee | Radius 25 |
| **LOD4** (Event-Only) | Nie (passiv) | Keine | Keine |

**LOD-Wechsel:** Ein Entity wechselt automatisch:
- LOD0 → LOD1: wenn `DistanceFromPlayer > 500` und `Duration > 5s`
- LOD1 → LOD2: wenn `DistanceFromPlayer > 1000` und `Duration > 5s`
- LOD2 → LOD3: wenn `DistanceFromPlayer > 2000` und `Duration > 5s`
- LOD3 → LOD4: wenn `DistanceFromPlayer > 4000`
- **Jeder → LOD0:** wenn `DistanceFromPlayer < 50` oder `Entity.IsVisible()`

---

## 10. EDGE CASES

### 10.1 "Alles ist weg"

Wenn keine Ressourcen in Perzeption:
- Entity wechselt zu `Explore` (zufällige Richtung, randomisiert Seed)
- Exploriert für `ExploreDuration = 10-30 Ticks` (randomisiert)
- Danach wieder `EvaluateAndExecute()`
- Wenn nach 5 Explore-Zyklen immer noch nichts: `Flee` von aktuellem Chunk (wenn keine Ressourcen im Chunk)

### 10.2 "Zwei Entities wollen dasselbe"

Konflikt-Lösung:
1. **First-Come-First-Served**: wer zuerst perzepiert, kriegt die Ressource
2. **Utility-Bias**: wer höheren Need-Druck hat, krigt Vorrang
3. **Social-Memory-Bias**: Freunde teilen sich (Social > 0.5 → teilen)
4. **Random Fallback**: wenn alles gleich → zufällig

### 10.3 "Entity ist blockiert"

Wenn Movement blockiert (Kollision):
- `WaitTick` = 3 Ticks (kurz warten)
- Wenn immer noch blockiert: `Explore` (andere Richtung versuchen)
- Wenn nach 5× Explore immer noch blockiert: `Flee` von Position

### 10.4 "Alle Resources sind leer"

- Entity verlässt Chunk
- Navigiert zu nächstem remembered Resource-Spot
- Wenn nicht remembered: random Explore

---

## 11. DEBUGGING / INSPEKTION

Für @der-tools-ui-programmer:

### 11.1 Decision-Trace (was hat die KI entschieden?)

```cpp
// UAetherisDecisionTrace — speichert letzten Entscheidungs-Zyklus
USTRUCT()
struct FDecisionTrace
{
    double Timestamp;
    float OverallPressure;
    int8 MostPressingNeed;
    TArray<FActionEvaluation> EvaluatedActions;  // Alle Actions mit Utility
    UAetherisAction* SelectedAction;
    float SelectedUtility;
    bool Interrupted;
};
```

Jedes Entity loggt seinen letzten Entscheidungs-Zyklus. Der Inspector kann anzeigen:

```
Entity: "Unit_001" (Hunger:0.1, Energy:0.6, Social:0.7)
Decision: "Sleep" (Utility: 0.375)
Evaluated: Eat(0.360), Sleep(0.375), Socialize(0.097)
Last Decision: Frame 15234, Time 253.67
Interrupted by: OnWeatherChanged(Storm) → jetzt Flee
```

### 11.2 Blackboard-Logging

```cpp
// IAetherisTickable.BlackboardLogging()
void BlackboardLogging(FString& OutLog) const
{
    OutLog = FString::Printf(
        TEXT("Entity[%lld] Hunger=%.2f Energy=%.2f Social=%.2f "
             "Pressure=%.3f Action=%s"),
        EntityId,
        NeedsComponent->CurrentHunger,
        NeedsComponent->CurrentEnergy,
        NeedsComponent->CurrentSocial,
        NeedsComponent->GetOverallPressure(),
        *GetName(DecisionComponent->GetActiveAction())
    );
}
```

---

## 12. SPECS FÜR SPÄTERE PHASEN (Out of Scope für Phase 1)

| Phase | Feature | Status |
|---|---|---|
| Phase 2 | **Schwarm-Verhalten** (Flocking, Boids) | → Needs-Bias: Social-Need führt zu Annäherung |
| Phase 3 | **Lernen** (Positive/Negative Reinforcement) | → Memory Valence als Lern-Signal |
| Phase 4 | **Rollen-basierte Variation** | → Role modifiziert Utility-Gewichte |
| Phase 5 | **Skill-System** | → Actions können Skills erfordern |
| Phase 6 | **Relationship-System** | → Social Memory wird Relationship-Graph |

---

## 13. OPEN QUESTIONS

| Frage | Optionen | Empfehlung | Offen für |
|---|---|---|---|
| **Utility-Gewichte hardcoded oder DataTable?** | DataTable | DataTable | @der-weltenbauer |
| **PerceptionRadius statisch oder dynamisch?** | Statik (150) | Statik für Phase 1 | @der-ingenieur (Performance) |
| **Decision-Interval: fest oder variabel?** | Variabel (druck-basiert) | Variabel | @der-ingenieur |
| **Flee-Priorität: immer oder nur bei bestimmter Gefahr?** | Immer | Immer (Safety First) | — |
| **Socializing: random oder gezielter Freund?** | Random in Perzeption | Random für Phase 1 | @der-weltenbauer |

---

## 14. QUELLEN

1. **Bakkes, M. et al.** — "Utility AI for Game Characters" (Game AI Pro, Vol. 1)  
2. **Isbittner, M.** — "Behavior Trees in games, AI, robotics, and beyond" (AI Magazine 2012)  
3. **Bonabeau, E. et al.** — "Swarm Intelligence: From Natural to Artificial Systems" (1999)  
4. **Kahneman, D.** — "Thinking, Fast and Slow" (System 1 vs System 2 → Utility AI als System 2, Flee als System 1)  

---

**ZUSAMMENFASSUNG FÜR PHASE 1:**

Ein Entity macht alle **N Ticks**:
1. Needs prüfen
2. Umgebung scannen
3. Alle verfügbaren Actions bewerten (Utility AI)
4. Beste Action wählen und ausführen
5. Auf Interrupts (Gefahr, Weather) reagieren

**5 Kernregeln > 50 Regeln.**  
**Emergenz aus Interaktion, nicht aus Komplexität.**  
**Alles konfigurierbar über DataAssets.**
