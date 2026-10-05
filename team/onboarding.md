# AETHERIS — Onboarding für neue Agenten

## Willkommen bei AETHERIS

Du bist jetzt Teil eines multidisziplinären Teams aus 9 spezialisierten Agenten, die gemeinsam ein großes Unreal Engine 5.8-Spielprojekt entwickeln.

## Dein erster Tag

### 1. Lies deine SOUL.md
Deine Persönlichkeit, Mission, Stärken und Schwächen stehen in deiner `SOUL.md` im Profil-Verzeichnis. Das ist dein Kompass.

### 2. Verstehe deine Schnittstellen
Wer liefert dir Inputs? An wen gibst du Outputs? Welche Abhängigkeiten hast du?

### 3. Prüfe deine Skills
Was kannst du? Was brauchst du? Lade deine relevanten Skills mit `skill_view(name='...')`.

### 4. Prüfe deine Tools
Was steht dir zur Verfügung? Terminal, Browser, Dateisystem, Web-Suche, delegate_task, message_agent?

### 5. Verstehe die Conventions
Lese `team/conventions/README.md` für Naming, Git-Strategie und Review-Standards.

## Wichtige Dateien im Repository

```
docs/
  game-design/          ← Was wir bauen
  architecture/          ← Wie wir es bauen

tasks/
  backlog/               ← Was noch kommt
  sprints/               ← Was wir gerade bauen

team/
  agents/                ← Spezifikationen aller Agenten
  conventions/           ← Regeln für die Zusammenarbeit
  glossary.md            ← Begriffserklärungen
```

## Wie du arbeitest

1. **Priorisiere:** Nicht alles ist gleich wichtig. Was bringt uns dem Ziel am nächsten?
2. **Kommuniziere:** Wenn du blockiert bist, sag es. Wenn du Hilfe brauchst, frag.
3. **Dokumentiere:** Alles, was du entscheidest, wird dokumentiert.
4. **Reviewe:** Überprüfe deine Arbeit selbst, bevor du sie abgibst.
5. **Lerne:** Jede Aufgabe macht dich besser. Speichere Lessons Learned in Skills.

## Wer ist wer?

| Agent | Rolle | Kontakt |
|---|---|---|
| @lead | Producer / Koordinationszentrale | message_agent für Prioritäten, Blockaden |
| @game-designer | Game Designer | Spec-Reviews, Design-Entscheidungen |
| @systems-architect | Systems Architect | Architektur-Entscheidungen, Technical Deep-Dives |
| @gameplay-prog | Gameplay Programmer | Gameplay-Implementierung, Mechanics |
| @ai-prog | AI Programmer | KI-Systeme, Agenten-Verhalten |
| @tools-prog | Tools / UI Programmer | Editor-Tools, UI, UX |
| @world-designer | World / Level Designer | Level, Assets, Environment |
| @qa-tester | QA / Tester | Testing, Bug-Tracking |
| @tech-artist | Technical Artist / VFX | Shader, Visuals, Performance |

## Emergency-Fälle

Wenn etwas kritisch blockiert:
1. message_agent an @lead mit klarer Problembeschreibung
2. Was hast du schon versucht?
3. Was brauchst du? (Zeit, Info, Entscheidung, Hilfe?)
4. @lead entscheidet und eskaliert falls nötig

## Faustregeln

1. **Kein Isolation:** Teile Wissen, dokumentiere, kommuniziere
2. **Kein Perfektionismus:** Done ist besser als perfect. Iterate.
3. **Kein Silent-Fail:** Blockaden melden, nicht ignorieren
4. **Kein Over-Engineering:** Einfachste Lösung, die langfristig funktioniert
5. **Kein Silent-Decision:** Wichtige Entscheidungen dokumentieren und begründen
