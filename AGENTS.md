# AGENTS für OFM-VirtualButton

## Ziel

Dieses Modul bildet virtuelle Taster auf Binäreingängen (integriert oder
extern) ab — Voraussetzung ist nur, dass die Quelle bei Betätigung ein
EIN und beim Loslassen ein AUS liefert. Sinnvoll, wenn die Funktionen des
vorhandenen physischen Tasters nicht ausreichen (z. B. Mehrfach-Klick,
Extra-Lang, dynamische DPT3-Richtung).

## Basis: OGM-Common

Dieses OFM baut auf `../OGM-Common/AGENTS.md` auf (Modulsystem, LED-/
Flash-/Zeit-API, Logging, Embedded-Rahmenbedingungen, Code-Konventionen)
— dort beschriebene Regeln und vorhandene Claude-Skills/Agents (z. B.
das Kanalauswahl-Muster) gelten auch hier.

## Prefix und Namenskonventionen

- **Modulprefix**: `BTN`
- **C++-Klassen**: `VirtualButtonModule` (Modul, `name()` liefert
  `"VirtualButton"`), `VirtualButtonChannel` (Kanal)
- **ETS-Anzeigename**: `Virtuelle Taster`, **Icon**: `button-pointer`

## Kanalanzahl

Die Kanalanzahl wird durch das OAM über `BTN_ChannelCount` vorgegeben.
Jeder Kanal hat **zwei** Eingänge (Index 0/1, `_buttonState[2]` /
`_buttonParams[2]`) — z. B. für 1-fach- vs. 2-fach-Taster-Betrieb an
einem Kanal. `processInputKo()` reicht eingehende KOs an
`processInputKoInput()`/`processInputKoLock()`/`processInputKoStatus()`
der Kanäle weiter.

## Tasten-Logik (`VirtualButtonChannel`)

- **Presse/Loslassen**: `processPress()`/`processRelease()`/
  `processPressAndHold()` unterscheiden kurzen, langen und
  "Extra-Lang"-Druck über `reactionTimeLong`/`reactionTimeExtraLong`.
- **Mehrfach-Klick**: `processMultiClick()` zählt Klicks innerhalb von
  `reactionTimeMultiClick`, bis zu `BTN_MaxMuliClicks` (3) — jeder
  Klickstand hat sein eigenes KO (`MultiClickParams`), zusätzlich ein
  optionaler Klickzähler (`multiClickCount`) für externe Logikauswertung.
- **Ausgabe-DPTs**: Kurz-/Lang-/Extra-Lang-Druck und Mehrfach-Klick haben
  jeweils einen eigenen konfigurierbaren DPT
  (`outputShortDpt`/`outputLongDpt`/`outputExtraLongDpt`/
  `outputMultiClickDpt`) aus der Menge DPT1/2/3.007/3.008/5.001/5.005/
  7.001/18.001 (`BTN_DPT*`-Defines).
- **Dynamischer Status** (`evaluateDynamicStatus()`,
  `processDynamicStatusTimer()`): für DPT3 (Dimmen/Rollladen) wird die
  Richtung situativ aus dem zuletzt bekannten Status abgeleitet, nicht
  fest konfiguriert.
- **Sperre**: `_lock` blockiert `processPress()`/`processRelease()`
  komplett, wird über `processInputKoLock()` gesetzt.

## Regeln für Weiterentwicklung

1. Neue Ausgabe-DPTs bekommen ein eigenes `BTN_DPT*`-Define und einen
   Zweig in `writeOutput()`.
2. `VirtualButtonModule` verwaltet nur das Channel-Array und
   `freeLoopIterate()` für den kooperativen Loop — keine Tasten-Logik
   dort.
3. Presse-, Mehrfach-Klick- und Status-Logik strikt in den jeweils
   passenden `process*()`/`event*()`-Methoden von `VirtualButtonChannel`
   halten.

## Dokumentation und Hilfe

- Dokumentation liegt in `doc/Applikationsbeschreibung-Taster.md`
- Jede sichtbare `ParameterRefRef` bekommt einen `HelpContext` mit
  Prefix `BTN-` (Ausnahme: `Empty` für Parameter ohne eigene Hilfeseite)
- Baggages werden über VS Code Task "OpenKNXproducer Documentation"
  erzeugt (`.vscode/tasks.json`)
- Deutsche Texte mit echten Umlauten (ä, ö, ü, ß) schreiben

## Referenzen

- [README.md](README.md) — Modus, DPTs, Funktionsüberblick
- [doc/Applikationsbeschreibung-Taster.md](doc/Applikationsbeschreibung-Taster.md) — vollständige Parameterbeschreibung
