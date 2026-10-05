# AETHERIS — Conventions

## Allgemein

### Dokumenten-Namen
- Lowercase mit Bindestrichen: `game-design-document.md`
- Keine Sonderzeichen, keine Umlaute in Dateinamen
- Versionierung nicht im Dateinamen, sondern im Git-Commit

### Ordner-Namen
- Lowercase, singular, beschreibend: `docs/`, `tasks/`, `scripts/`
- Keine Leerzeichen, keine Bindestriche

### Kommentare
- In Code: Englisch (Internationalität)
- In Docs: Deutsch (AETHERIS intern) — oder nach Absprache Englisch
- Commit Messages: Conventional Commits (siehe unten)

## Git Conventions

### Branch-Namen
```
feature/kurz-beschreibung
bugfix/kurz-beschreibung
hotfix/kurz-beschreibung
docs/änderung
refactor/bereich
```

### Commit Messages (Conventional Commits)
```
type(scope): Beschreibung

types:
  feat:     Neues Feature
  fix:      Bugfix
  docs:     Dokumentation
  style:    Formatierung (kein Code-Change)
  refactor: Code-Umstrukturierung (kein Feature, kein Fix)
  test:     Tests
  chore:    Build, Dependencies, CI
  perf:     Performance-Verbesserung
  revert:   Revert eines Commits
```

Beispiel:
```
feat(ai): flock behavior for bird entities
fix(gameplay): player health regen rate
docs: update architecture diagram
```

### PR-Beschreibung
Jeder PR muss enthalten:
- Was wurde geändert?
- Warum?
- Wie wurde es getestet?
- Screenshot/Video bei visuellen Changes
- Link zum zugewiesenen Ticket

## Code Conventions

### Python (Hermes Scripts)
- PEP 8 Konformität
- Type Hints wo sinnvoll
- Docstrings für jede öffentliche Funktion
- Max. 120 Zeichen pro Zeile
- Black für Formatierung

### Markdown (Dokumente)
- Maximale Zeilenlänge: 100 Zeichen
- GFM (GitHub Flavored Markdown)
- Tables für strukturierte Daten
- Checkboxes für Aufgaben

### UE5 C++ (später)
- Unreal Engine Coding Standard
- Pimpl für Header-Sparsity
- Smart Pointer vor Raw Pointer
- UE_LOG für Debugging

## Review-Checkliste

### Code Review
- [ ] Code funktioniert wie spezifiziert
- [ ] Keine Security-Loops (Secrets, Injection)
- [ ] Fehlerbehandlung vorhanden
- [ ] Keine hardcoded Werte
- [ ] Tests vorhanden und bestanden
- [ ] Code ist lesbar und selbsterklärend
- [ ] Keine TODOs ohne Ticket

### Design Review
- [ ] Passt zur Game Vision
- [ ] Skalierbar und erweiterbar
- [ ] Performance im Budget
- [ ] User Experience konsistent
- [ ] Kein "Over-Engineering"
