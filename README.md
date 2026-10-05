# AETHERIS — Projektstruktur

```
AETHERIS/
├── docs/                    # Design-Docs, GDD, Architektur-Dokumente
│   ├── game-design/         # Game Design Document, Mechanics, Rules
│   ├── architecture/        # System-Architektur, UE5-Setup, Pipelines
│   ├── worldbuilding/       # Lore, Locations, Charaktere, Fraktionen
│   └── production/          # Sprint-Pläne, Meilensteine, Budget
├── tasks/                   # Task-Tracking, Ticket-Struktur
│   ├── sprints/             # Sprint-Pläne & Reviews
│   ├── backlog/             # Unpriorisierte Tasks
│   └── epics/               # Große Features als Epic-Zusammenfassung
├── reports/                 # Weekly Reports, Meeting Minutes, Status
│   ├── weekly/              # Wöchentliche Producer-Reports
│   ├── meetings/            # Meeting-Protokolle mit Action Items
│   └── reviews/             # Code Reviews, Design Reviews, Sprint Reviews
├── unreal/                  # UE5-Projekt (wird später hier gemountet)
│   ├── Source/              # C++ Code
│   ├── Content/             # Assets, Maps, Blueprints
│   ├── Config/              # Project Settings
│   └── Plugins/             # Custom Plugins
├── scripts/                 # DevOps, Build, Automation
│   ├── ci/                  # CI/CD Pipelines (GitHub Actions)
│   ├── setup/               # Setup-Skripte
│   └── tools/               # Hilfs-Tools
└── team/                    # Team-Spezifikationen, Agent-Configs
    ├── agents/              # SOUL.md, Skill-Sets, Tool-Configs pro Agent
    ├── onboarding.md        # Neue Team-Mitglieder (Agenten) einweisen
    ├── conventions/         # Coding Conventions, Naming, Standards
    └── glossary.md          # Fachbegriffe, Begrifflichkeiten AETHERIS
```
