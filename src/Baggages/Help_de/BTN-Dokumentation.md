### Dokumentation

Dieses Modul stellt virtuelle Taster bereit. Voraussetzung ist, dass beim Tastendruck ein EIN und beim Loslassen ein AUS gesendet wird.

#### Modus
- 1fach-Taster
- 2fach-Taster
- Mehrfach-Klick

#### DPTs
- DPT 1.001 Schalten
- DPT 2.001 Zwangsführung
- DPT 3.007 Dimmen
- DPT 3.008 Jalousie
- DPT 5.005 (0..255)
- DPT 5.001 (0..100%)
- DPT 7.001 (0..65535)
- DPT 18.001 Szene

#### Funktionen
- Sperrmöglichkeit
- Individuelle Reaktionszeiten
- Unterschiedung zwischen "Drücken" und "Loslassen"
- Es gibt einen Zusätzlichen DPT1 AUsgab bei 1/2fach-Tastern
- Zusätzliches "Extra-Lang" neben den üblichen Einfacher und Langer Tastendruck.
- Für DPT1.001 gibt es einen "Umschalter"
- Bei DPT3.007 (Dimmer) gibt es einen helligkeitsabhängigen Status, welcher nach einer Rückfallzeit anhand eines Schwellwertes die nächste Dimmerrichung vorgibt.
- Für die Kombination von Mehrfach-Klick und DPT1.001 gibt es je Mehrfach-Klick ein eigens KO. In Kombination mit Logikmodul können auch spezielle Anforderungen abgebildet werden.
- Bei Mehrfach-Klick kann auch der Klickzähler ausgegeben werden, um diese in einer externen Logik über den 3fach Klick hinaus ausgewertet zu werden.

Das Repository findet man unter: https://github.com/openknx/OFM-VirtualButton