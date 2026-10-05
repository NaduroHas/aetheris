# AETHERIS — Master Project Analysis

**Stand:** 05. Oktober 2026  
**Status:** Projekt initialisiert nach Bible- und Project-Scan  
**Zugrundegelegt:** Project Bible v1.0 (877 Zeilen, 40 Kanonische Regeln, 8 Hermes-Regeln) + AETHERIS.uproject (UE 5.8)

---

## 1. Projekt-Status

| Kriterium | Status | Bewertung |
|---|---|---|
| UE5.8 Projektdatei | ✅ Vorhanden | `EngineAssociation: 5.8` |
| Source-Ordner | ⚠️ Nur Vorlagen-Code | `Source/Test1/` existiert, aber leer (nur Animation-Referenzen) |
| Content-Ordner | ⚠️ Nur externe Assets | 54.150 Assets, alles externe Packs (Paragon, Fishermans_Cabin etc.) |
| AETHERIS-eigener Code | ❌ Nicht vorhanden | Kein C++, kein Blueprint-Gerüst, kein Simulation Core |
| Tests | ❌ Nicht vorhanden | Kein Tests/ Verzeichnis |
| Benchmarks | ❌ Nicht vorhanden | Kein Benchmarks/ Verzeichnis |
| Plugins | ✅ Voll konfiguriert | 40 Plugins aktiv, davon 14 AI-relevant |
| Config | ✅ Standard | DefaultEngine/Game/Input.ini vorhanden |
| Projektgröße | 111,7 GB | Überwiegend externe Asset-Packs |

### Fazit: Leeres UE5-Projekt mit Asset-Füller, null AETHERIS-Code

Das Projekt ist technisch funktionsfähig (UE5.8 + 40 Plugins), aber inhaltlich **ein leeres Gerüst**. Alle 54.000+ Assets stammen aus externen Packs. Es gibt keinen AETHERIS-eigenen Source-Code, keine Simulation, keine Game-Play-Logik.

---

## 2. AI-Relevante Plugins (14 identifiziert)

| Plugin | Kategorie | Bedeutung für AETHERIS |
|---|---|---|
| **HTNPlanner** | KI-Planung | Hierarchical Task Networks — essentiell für Agent-Verhaltensplanung |
| **MassAI** | Massentier-KI | Unreal's ECS-basierte Mass-AI für tausende Einheiten |
| **MassCrowd** | Massentier-KI | Crowd-Simulation auf Mass-System-Basis |
| **GameplayStateTree** | KI-Verhalten | UE5.8 State Trees — modernes KI-Verhaltenssystem |
| **GameplayBehaviors** | KI-Verhalten | Behavioral Trees + State Trees Integration |
| **AIAssistant** | KI-Hilfsmittel | Assistent-System für KI-Entwicklung |
| **AIModuleToolset** | KI-Hilfsmittel | AI-Module-Tools |
| **LearningAgents** | Reinforcement Learning | ML-basiertes Agent-Learning |
| **MLAdapter** | ML-Integration | Adapter für externe ML-Frameworks |
| **HairCardGenerator** | VFX/Asset | Charakter-Generierung |
| **HairModelingToolset** | VFX/Asset | Haar-Modellierung |
| **MeshTerrainMode** | Worldbuilding | Terrain-Modellierung |
| **HairStrandsMutable** | VFX | Dynamische Haar-Strähnen |
| **ExampleCustomDataInterface** | Data | Beispiel für Custom Data-Interfaces |

### Kritische Plugins für Core-Entwicklung:
- **HTNPlanner** — Unser primäres KI-Planungssystem (Utility + HTN Hybrid)
- **MassAI/MassCrowd** — Massentier-KI für LOD 3-4 (Background Simulation)
- **GameplayStateTree** — Individual-Verhaltens-Bäume für LOD 0-2
- **LearningAgents** — Langfristiges Lernen/Adaption von Units

---

## 3. Project Bible — Kernziele (extrahiert)

### Vision
> "Eine Welt, in der der Spieler zehn Stunden zusieht und Geschichten entstehen, die er nicht vorher geschrieben hat." — Anhang C

### Technische Ziele
| Parameter | Wert |
|---|---|
| Engine | UE5.8 |
| Spielmodus | Singleplayer |
| Ziel-Units | 1.000–10.000 (Prototyp) |
| Weltgröße (Start) | ~2×2 km |
| Simulation | Deterministisch, seedbar, headless-fähig |
| LOD | 5 Stufen (0-4: Full → Background → Historical) |
| Zeitsteuerung | Pause/1×/5×/20×/100× |

### Architektur-Prinzipien
1. **Simulation Core** ist vollständig vom Rendering getrennt
2. **Emergenz** vor Skripting — kein "CreateVillage"-Quest
3. **Performance** ist Design-Parameter, kein nachträglicher Patch
4. **Explainability** wichtiger als perfekte Black-Box-Komplexität
5. **Historische Persistenz** — Verstorbene Units bleiben vollständig dokumentiert

### 17 Entwicklungsphasen
| Phase | Name | Primary Deliverable |
|---|---|---|
| 0 | Foundation | Repo, Build, Core IDs, logging, tests, seed |
| 1 | Individual | Unit data, identity, needs, basic lifecycle |
| 2 | Simulation Core | Scheduler, time, event bus, save state |
| 3 | World | Terrain, water, biome, resources |
| 4 | Ecology | Plants, animals, food chain |
| 5 | Skills | XP, learning, teaching |
| 6 | Social | Relationships, family, households, groups |
| 7 | Settlement | Buildings, construction, ownership, local economy |
| 8 | Society | Institutions, reputation, politics, culture |
| 9 | Peoples | Humans/Goblins/Demons and differences |
| 10 | Combat | Combat, injuries, war |
| 11 | Magic | Magic system and integration |
| 12 | Bosses | Boss/Brandmal |
| 13 | God Player | God tools, intervention, possession |
| 14 | Vertical Slice | End-to-end emergent world |
| 15 | Alpha | Performance, stability, content breadth |
| 16 | Beta | Balance, UX, persistence, polish |

---

## 4. Gap-Analyse: Bible vs. Realität

| Bible-Anforderung | Aktueller Status | Kritikalität |
|---|---|---|
| **Phase 0: Foundation** — Repo, Build, Core IDs, logging, tests, seed | ❌ Nicht begonnen | 🔴 KRITISCH |
| **Simulation Core** — Scheduler, Event-Bus, State Management | ❌ Nicht vorhanden | 🔴 KRITISCH |
| **Entity Registry** — Stabile IDs für alle Entitäten | ❌ Nicht vorhanden | 🔴 KRITISCH |
| **Individual System** — Unit-Zustände, Needs, Lebenszyklus | ❌ Nicht vorhanden | 🔴 KRITISCH |
| **Decision System** — Rules + Utility | ❌ Nicht vorhanden | 🟡 HOCH |
| **Presentation Layer** — Actors, Animation, UI | ⚠️ Nur externe Assets | 🟡 HOCH |
| **God Interface** — Spieleraktionen | ❌ Nicht vorhanden | 🟡 HOCH |
| **Debug Layer** — Inspector, Tracing, Replays | ❌ Nicht vorhanden | 🟢 MITTEL |
| **Performance Budgets** — CPU/RAM-Dokumentation | ❌ Nicht vorhanden | 🟡 HOCH |

### Priorisierte Action Items

**SOFORT (Phase 0):**
1. Source-Gerüst erstellen: `AetherisCore`, `AetherisSimulation`, `AetherisGameplay`, `AetherisTools`
2. Core IDs implementieren (`UnitID`, `SettlementID`, etc.)
3. Logging-System aufsetzen
4. Basis-Build-Pipeline testen (VS-Solution + Module)
5. Seed-basierte Welt-Generierung (Test)
6. Test-Framework konfigurieren

**KURZFRISTIG (Phase 1-2):**
7. Individual System: Unit-Data-Struktur, Needs-System
8. Scheduler + Event-Bus
9. Save/Load-Grundgerüst

---

## 5. Content-Analyse — Asset-Packs

| Pack | Assets | Typ | Nutzbart für AETHERIS? |
|---|---|---|---|
| `__ExternalActors__` | 4.332 | Various characters | ⚠️ Teilweise als Platzhalter |
| `Fishermans_Cabin` | 1.259 | Environment + Blueprints | ✅ Guter Start für Prototyp |
| `BattleWizardPBR` | 76 | Character + Animations | ⚠️ Als Spieler-Charakter möglich |
| `Kentaur` | 82 | Creature (Centaur) | ✅ Für Spezies-System |
| `Necropolis` | 950 | Environment + VFX | ✅ Für Ruinen-/Dungeon-System |
| Paragon-Serie | ~35.000 | Epic Characters | ❌ Lizenzprobleme, nicht nutzbar |
| Sonstige | ~12.500 | Mixed VFX, terrain | ⚠️ Teilweise nutzbare VFX/Maps |

### Empfehlungen:
- **Fishermans_Cabin** als Basis-Umgebung verwenden (vollständige Blueprints, Materialien, VFX)
- **Necropolis** für Ruinen-/Dungeon-Content (passend zu Kanonischer Regel: "Ruinen sind dauerhafte historische Elemente")
- Paragon-Assets als Lizenz-Risiko kennzeichnen — nicht produktiv verwenden
- Für Prototyp: UE5 Starter Content + Fishermans_Cabin kombinieren

---

## 6. Lead-Entscheidung: Nächste Schritte

### Option A — Phase 0: Foundation sofort beginnen
**Systems Architect** baut das Source-Gerüst, **Gameplay Programmer** testet die Build-Pipeline. Priorität: Kompilierbares Projekt mit Test-Framework in 1-2 Tagen.

### Option B — Game Designer erstellt Phase-0-GDD
Bevor Code geschrieben wird: konkretes Design für Phase 0-2 mit klaren Acceptance Criteria. **Game Designer** mit Bible-Referenz.

### Option C — AI Programmer: Mini-Prototyp
Ein autonomer Unit-Prototyp mit HTNPlanner als Proof-of-Concept. Zeigt ob KI-Systeme funktionieren, bevor wir das ganze Gerüst bauen.

---

## 7. Kanonische Regeln — Zusammenfassung (Top 10 kritisch)

1. **Siedlungen entstehen emergent** — kein "CreateVillage"-Schalter
2. **Eigentum** = Kontrolle + soziale Anerkennung + lokale Regeln
3. **Historische Wahrheit ≠ sozialer Ruf** — getrennt gespeichert
4. **Institutionen sind emergent** — entstehen aus Problemen, nicht Design-Template
5. **Kein vorgegebener Fortschrittsbaum** — Technologie emergent
6. **Religion ist Randsystem** — nicht Kernfokus
7. **Keine automatische Reset-Funktion** — Folgen bleiben
8. **Verstorbene Units bleiben historisch erhalten** — voll dokumentiert
9. **Performance ist Design-Parameter** — kein nachträglicher Patch
10. **Wenn emergentes Verhalten nicht debuggbar ist → Entwicklungsproblem**

---

*Diese Analyse ist die kanonische Grundlage für alle weiteren Entscheidungen. Jeder Agent liest diese Datei vor der Arbeit.*
