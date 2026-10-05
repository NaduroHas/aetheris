# AETHERIS — Kanban-Board Struktur

## Board-Konfiguration

**Board:** AETHERIS Development Board  
**Typen:** Epic, Feature, Task, Bug, Spike, Review  
**Spalten:** Backlog → Ready → In Progress → In Review → Done

## Spalten-Beschreibung

### Backlog
- Alle unerledigten Items
- Keine Priorisierung
- Nur für den Producer sichtbar relevant

### Ready (Sprint Planning)
- Items, die für den nächsten Sprint bereit sind
- Klare Beschreibung, Akzeptanzkriterien, Schätzung
- Vom Producer priorisiert

### In Progress
- Wird aktuell bearbeitet
- Max 2-3 Items pro Agent gleichzeitig
- Blockaden müssen dokumentiert sein

### In Review
- Abgabe zur Prüfung (Code, Design, QA)
- Reviewer zugewiesen
- Feedback wird eingetragen

### Done
- Abgenommen und abgeschlossen
- Akzeptanzkriterien erfüllt
- Merge/Integration erfolgt

## Ticket-Vorlagen

### Feature Ticket
```
## Titel: [Feature] Kurze Beschreibung
## Agent: Zugewiesen an
## Priority: P0-P3
## Estimate: Story Points

### Beschreibung
Was soll gebaut werden?

### Akzeptanzkriterien
- [ ] Kriterium 1
- [ ] Kriterium 2

### Abhängigkeiten
- Benötigt: [andere Tickets]

### Notes
```

### Bug Ticket
```
## Titel: [Bug] Symptom
## Agent: Zugewiesen an
## Priority: P0-P3
## Severity: Critical/Major/Minor

### Reproduction
1. Schritt 1
2. Schritt 2

### Erwartet vs. Tatsächlich

### Logs / Screenshots
```

### Spike Ticket
```
## Titel: [Spike] Forschungsfrage
## Agent: Zugewiesen an
## Zeitlimit: Max X Stunden

### Frage
Was soll herausgefunden werden?

### Deliverable
- Technischer Report
- Proof of Concept
- Empfehlung (Go / No-Go)
```

## Sprint-Struktur

**Sprint-Länge:** 2 Wochen  
**Planning:** Montag der ersten Woche  
**Review:** Freitag der zweiten Woche  
**Retrospective:** Montag nach der Review

## Prioritäten

| Level | Bedeutung | Aktion |
|---|---|---|
| P0 | Blockiert alles | Sofort, heute, jetzt |
| P1 | Wichtig für nächsten Meilenstein | Dieser Sprint |
| P2 | Wünschenswert, aber nicht kritisch | Nächster Sprint |
| P3 | Nice-to-have | Backlog, wenn Zeit |
