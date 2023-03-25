### Wert beim Drücken

Der ausgewählte Wert wird beim Drücken sofort bzw. beim gedrückthalten (beim langem oder extra langen Tastendruck) nach Ablauf der eingestellten Reaktionszeit auf den Bus gesendet.

Dadurch werden prinzipbedingt auch alle aktiven Events (kurz, lang, extralang) unabhänig vom Loslassen ausgeführt. In der Regel wird daher beim "Einfacher Tastendruck" garnicht mit diesem Event gearbeitet. Beim gedrücktem Taster sollte man auf dieses Event nur reagieren, wenn es das letzte Event ist.

Beispiel:
Möchte ich ein Gerät mit kurzem Tasendruck einschalten und mit langem Tastendruck ausschalten, so sollte folgene Konfiguration gewählt werden:

Beim "Einfacher Tastendruck" sollte mittels "Wert beim Loslassen" ein EIN geschickt werden.
Beim "Langer Tastendruck" sollte mittels "Wert beim Drücken" ein AUS geschickt werden.

In dieser Kombination ist sichergestellt, dass das EIN nur beim Loslassen und innerhalb der Reaktionszeit gesendet wird.
Das AUS hingegen wird sofort bei erreichen der eingestellten Reaktionszeit ausgelöst, so dass der Benutzer sieht das er loslassen kann.
