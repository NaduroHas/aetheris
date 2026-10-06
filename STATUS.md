# AETHERIS — DEVELOPMENT STATUS

## Current state

**Engine target:** Unreal Engine 5.8  
**Design authority:** AETHERIS Master Project Bible v1.0  
**Repository:** NaduroHas/aetheris  
**Active foundation branch:** `feature/foundation-simulation-core`  
**Foundation PR:** #5

## Implemented foundation

- Deterministic Core IDs and logging
- Simulation Manager with continuous simulation time
- Pause / Resume / 1x–100x speed multiplier
- Stable non-zero entity IDs
- Entity registry with ID → index mapping
- Simulation-only Individuals independent of Actors
- Human / Goblin / Demon prototype species
- Male / Female prototype sex model
- Individual biological state: age, hunger, thirst, social need, fatigue, health
- LOD state placeholder
- Deterministic entity scheduler with configurable tick intervals
- Deterministic world seed initialization
- Persistent in-memory event history with stable event IDs
- Individual personality foundation
- Skill XP / level / potential foundation
- Knowledge with source, trust and accuracy belief
- Multi-level goals
- Hard-constraint + utility-based action evaluation foundation

## Intentionally not claimed as complete

The full game is **not yet production-complete**. The Master Bible defines further systems including:

1. World / terrain / water / biome
2. Ecology and animals
3. Perception and attention
4. Memory
5. Communication / lies / secrets
6. Learning and teaching
7. Relationships / family / households / groups
8. Buildings / construction / destruction
9. Ownership / resources / economy / logistics
10. Settlements / institutions / politics / culture
11. Humans / Goblins / Demons differentiation
12. Combat / war / injuries
13. Magic / monsters / bosses
14. God-player tools
15. LOD 0–4 background simulation
16. Save/load and versioned persistence
17. Debug inspectors / replay / headless runner
18. Vertical slice, alpha and beta validation

## Quality rule

A feature is only considered complete when it satisfies the Master Bible Definition of Done: design goal, correct state, interactions, persistence, tests, regression checks, performance where relevant, inspectability, documentation and explainability.

## Current next priority

Build the simulation outward from the Individual layer:

**Individual → Needs/Health → Perception → Memory/Knowledge → Goals → Decision → Action → Social → Settlement → Economy → Civilization → History**

No system should bypass the Simulation Core or require a rendered Actor to exist.
