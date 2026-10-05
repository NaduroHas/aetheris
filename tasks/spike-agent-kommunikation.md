# SPIKE: Agent-Kommunikation & Task-Delegation

## Ziel
Nachweisen, dass AETHERIS-Agenten tatsächlich miteinander kommunizieren und Tasks delegieren können. Kein Design, kein Code — nur die Infrastruktur.

## Hypothesen zu prüfen
1. `message_agent` funktioniert zwischen allen Agent-Paaren
2. `delegate_task` kann Subagenten mit Kontext starten
3. Roundtrip: Lead schickt → Agent antwortet → Lead empfängt
4. Latenz und Fehlermarge im praktischen Betrieb

## Test-Plan

### Test 1: Direct Message (Lead → Gameplay Programmer)
- Lead schickt eine konkrete, einfache Aufgabe an @gameplay-programmer
- Spiel: "Beschreibe in 3 Sätzen, wie du einen Movement-Component in UE5.8 strukturieren würdest"
- Erwarten: Antwort vom Agenten mit seiner Perspektive

### Test 2: Task Delegation (Lead → AI Programmer)
- Delegate einer Subagent-Task an @ai-programmer
- Spiel: "Analysiere den aktuellen Projektstand und nenne 3 Blockaden, die ein AI Programmer sofort angehen würde"
- Erwarten: Structurierte Antwort mit konkreten Punkten

### Test 3: Cross-Team-Kommunikation (Lead → QA Tester)
- Lead schickt an @qa-tester
- Spiel: "Was würdest du als erstes testen, bevor der erste Gameplay-Code committed wird?"
- Erwarten: QA-Perspektive mit konkreten Test-Szenarien

## Erfolgskriterien
- [ ] Mindestens 1 Roundtrip erfolgreich (Message + Antwort)
- [ ] Mindestens 1 delegate_task erfolgreich
- [ ] Keine Systemfehler oder Authentifizierungsprobleme
- [ ] Antworten sind sachlich und rollenkonform

## Ausgabe-Format
Jeder Test wird dokumentiert mit:
- Gesendete Nachricht
- Erhaltene Antwort
- Latenz (ca.)
- Bewertung: ERFOLG / TEILERFOLG / FEHLER

---

*Erstellt: Lead | Status: IN PROGRESS*
