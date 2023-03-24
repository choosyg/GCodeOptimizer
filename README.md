# GCodeOptimizer
Optimiert Estlcam-GCode für Fräsbahnen mit schräg eintauchendem Werkzeug. Estlcam erzeugt für jede Ebene eine Rampe welche die Maschine vor und wieder zurück fährt. Das braucht gerade für dickes Material mit nicht sehr hohen Anbindungen und kleinen Teilen die haupt Fräszeit! Es ist notwendig um Gravuranfänge, Anbindungen, etc sauber zu fräsen. Es ist jedoch unnötig für alle Fräsebenen oberhalb von Anbindungen für Teile oder Ausschnitte mit geschlossenen Fräsbahnen.

Dieses Programm optimiert den GCode indem es die Rampen ersetzt. 

## ACHTUNG: 
* Der Eintauchwinkel wird aktuell verdoppelt!
* Benutzung auf eigene Gefahr! Das Programm ist nur so gut, dass es bisher bei mir aufgetauchte Fräsbahnen korrekt optimiert. Bevor ihr den optimierten GCode verwendet, solltet ihr in eurem GCode Prozessor also überprüfen, ob die Bahnen korrekt sind.

## Beispiel:

<table>
<tr>
<td> Estlcam </td> <td> Optimiert </td>
</tr>
<tr>
<td> 

```
(Nr. 1 Ausschnitt: Ausschnitt 1)
G00 X60.6464 Y30.1386
G00 Z0.5000
G01 Z0.0000 F1000 S24000
G02 X61.4073 Y33.0383 Z-2.4770 I0.4269 J1.4377
G01 X61.4488 Y33.0272 Z-2.5000
G01 X61.4073 Y33.0383 Z-2.5230
G03 X60.6464 Y30.1386 Z-5.0000 I-0.3340 J-1.4621
G02 X61.4073 Y33.0383 I0.4269 J1.4377
G01 X61.4584 Y33.0246
G02 X60.7397 Y30.1136 I-0.3845 J-1.4493
G01 X60.6464 Y30.1386
```

</td> <td>

```
(Nr. 1 Ausschnitt: Ausschnitt 1)
G00 X60.6464 Y30.1386
G00 Z0.5000
G01 Z0.0000 F1000 S24000
G2.000000 X61.407300 Y33.038300 I0.426900 J1.437700 Z-4.954000
G1.000000 X61.458400 Y33.024600 Z-5.000000
G02 X60.7397 Y30.1136 I-0.3845 J-1.4493
G01 X60.6464 Y30.1386
```

</td>
</tr>
</table>
