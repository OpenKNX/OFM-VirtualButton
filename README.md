# OFM-VirtualButton

Dies ist eine Library für virtuelle Taster zur Verwendung mit Binäreingängen oder weiteren Tastern mit weniger Funktionen. Voraussetzung ist, dass beim Tastendruck ein EIN und beim Loslassen ein AUS gesendet wird.

Ausgabetyp
- 1fach-Taster (mit Lang- und Extralang-Taster)
- 2fach-Taster (mit Lang- und Extralang-Taster)
- Doppel- & Dreifachklick (mit Lang-Taster)

DPTs
- DPT1.001: Normales Schalten & Umschalten
- DPT2.001: Zwangsführung
- DPT3.007: Dimmen (nur per Lang-Taster)
- DPT5.001: Prozent
- DPT5.005: Ganzzahl
- DPT17.001: Szenen (ohne Speichern)
  
Weitere Funktionen
- Sperre
- Grundeinstellung für Reaktionszeiten + abweichende Einstellung je Taster
- Verwendung von internen KOs ohne eine GA zuzuweisen.
- Beim einfachen Taster Aufgaben kann zwischen "Gedrückt" und "Losgelassen" unterschieden werden.
- Für die Kombination von "MultiClick" und DPT1.001 gibt es je Mehrfachklick ein eigens KO. In Kombination mit Logikmodul können auch spezielle Anforderungen abgebildet werden.
- Für DPT1.001 gibt es einen "Umschalter"
- Bei DPT3.007 (Dimmer) gibt es einen helligkeitsabhängigen Status, welcher nach einer Rückfallzeit anhand eines Schwellwertes die nächste Dimmerrichung vorgibt.


