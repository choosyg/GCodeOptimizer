# GCodeOptimizer
Optimiert Estlcam-GCode für Fräsbahnen mit schräg eintauchendem Werkzeug. Estlcam erzeugt für jede Ebene eine Rampe welche die Maschine vor und wieder zurück fährt. Das braucht gerade für dickes Material mit nicht sehr hohen Anbindungen und kleinen Teilen die haupt Fräszeit! Es ist notwendig um Gravuranfänge, Anbindungen, etc sauber zu fräsen. Es ist jedoch unnötig für alle Fräsebenen oberhalb von Anbindungen für Teile oder Ausschnitte mit geschlossenen Fräsbahnen.

Dieses Programm optimiert den GCode indem es Rampen welche unnötigerweise hin und zurück gehen ersetzt. Das Fräsprogramm enthält dann an diesen stellen keine 2 Stops mehr sondern fräst sauber durch was die Fräszeit teilweise erheblich reduziert. 

## ACHTUNG: 
* Benutzung auf eigene Gefahr! Das Programm ist nur so gut, dass es bisher bei mir aufgetauchte Fräsbahnen korrekt optimiert. Bevor ihr den optimierten GCode verwendet, solltet ihr in eurem GCode Prozessor also überprüfen, ob die Bahnen korrekt sind.

## Nutzung
* Aktuelle Version herunterladen: https://github.com/choosyg/GCodeOptimizer/releases/latest
* Evtl. musst du noch die Microsoft C++ Redistributables installieren: https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170
* GCode-Datei per Drag&Drop auf die GCodeOptimizer.exe ziehen -> neben der GCode-Datei wird das Ergebnis als optimized.nc gespeichert
* Alternativ kann das Programm auf der Kommandozeile über "GCodeOptimizer myGcode.nc" aufgerufen werden. Das Ergebnis wird im aktuellen Verzeichnis als optimized.nc gespeichert.
* Das Program gibt am Ende aus um wieviel schneller die Datei gefräst wird. Die Einheiten hängen von den Estlcam-Einstellungen ab. Ich habe mm/min eingestellt, die Zahlen sind dann Fräsdauer in Minuten. Für die Fräsdauer wird keine Trägheit/Beschleunigung der Maschine berücksichtigt. Es wird nur die Länge der Fräsbahn zusammen mit den gesetzten Geschwindigkeiten (F...) berücksichtigt. Die wahre Fräsdauer wird also etwas höher liegen.

## Beispiel:

<table>
<tr>
<td> Estlcam </td> <td> Optimiert </td>
</tr>
<tr valign="top">
<td> 

```
M03 S27000
G00 Z5.0000

(Nr. 1 Ausschnitt: Ausschnitt 1)
G00 X7.2412 Y6.0728
G00 Z0.5000
G01 Z0.0000 F1000 S27000
G02 X5.6527 Y8.3236 Z-0.2500 I1.7588 J2.9272
G03 X7.2412 Y6.0728 Z-0.5000 I3.3473 J0.6764
G02 X7.3444 Y11.9868 I1.7588 J2.9272
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X5.6527 Y8.3236 Z-0.7500 I1.7588 J2.9272
G03 X7.2412 Y6.0728 Z-1.0000 I3.3473 J0.6764
G02 X7.3444 Y11.9868 I1.7588 J2.9272
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X5.6527 Y8.3236 Z-1.2500 I1.7588 J2.9272
G03 X7.2412 Y6.0728 Z-1.5000 I3.3473 J0.6764
G02 X7.3444 Y11.9868 I1.7588 J2.9272
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X5.6527 Y8.3236 Z-1.7500 I1.7588 J2.9272
G03 X7.2412 Y6.0728 Z-2.0000 I3.3473 J0.6764
G02 X7.3444 Y11.9868 I1.7588 J2.9272
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X5.6527 Y8.3236 Z-2.2500 I1.7588 J2.9272
G03 X7.2412 Y6.0728 Z-2.5000 I3.3473 J0.6764
G02 X7.3444 Y11.9868 I1.7588 J2.9272
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X5.6527 Y8.3236 Z-2.7500 I1.7588 J2.9272
G03 X7.2412 Y6.0728 Z-3.0000 I3.3473 J0.6764
G02 X7.3444 Y11.9868 I1.7588 J2.9272
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X5.6527 Y8.3236 Z-3.2500 I1.7588 J2.9272
G03 X7.2412 Y6.0728 Z-3.5000 I3.3473 J0.6764
G02 X7.3444 Y11.9868 I1.7588 J2.9272
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X5.6527 Y8.3236 Z-3.7500 I1.7588 J2.9272
G03 X7.2412 Y6.0728 Z-4.0000 I3.3473 J0.6764
G02 X7.3444 Y11.9868 I1.7588 J2.9272
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X5.6527 Y8.3236 Z-4.2500 I1.7588 J2.9272
G03 X7.2412 Y6.0728 Z-4.5000 I3.3473 J0.6764
G02 X7.3444 Y11.9868 I1.7588 J2.9272
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X5.6527 Y8.3236 Z-4.7500 I1.7588 J2.9272
G03 X7.2412 Y6.0728 Z-5.0000 I3.3473 J0.6764
G02 X7.3444 Y11.9868 I1.7588 J2.9272
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X5.6527 Y8.3236 Z-5.2500 I1.7588 J2.9272
G03 X7.2412 Y6.0728 Z-5.5000 I3.3473 J0.6764
G02 X7.3444 Y11.9868 I1.7588 J2.9272
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X5.6527 Y8.3236 Z-5.7500 I1.7588 J2.9272
G03 X7.2412 Y6.0728 Z-6.0000 I3.3473 J0.6764
G02 X7.3444 Y11.9868 I1.7588 J2.9272
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G00 Z5.0000

(Nr. 2 Ausschnitt: Ausschnitt 32)
G00 X8.5178 Y8.1649 Z5.0000
G00 Z0.5000
G01 Z0.0000 F1000
G02 Y9.8352 Z-0.1781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-0.2500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-0.3219 I-0.3057 J-0.9146
G03 Y8.1649 Z-0.5000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-0.6781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-0.7500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-0.8219 I-0.3057 J-0.9146
G03 Y8.1649 Z-1.0000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-1.1781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-1.2500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-1.3219 I-0.3057 J-0.9146
G03 Y8.1649 Z-1.5000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-1.6781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-1.7500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-1.8219 I-0.3057 J-0.9146
G03 Y8.1649 Z-2.0000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-2.1781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-2.2500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-2.3219 I-0.3057 J-0.9146
G03 Y8.1649 Z-2.5000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-2.6781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-2.7500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-2.8219 I-0.3057 J-0.9146
G03 Y8.1649 Z-3.0000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-3.1781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-3.2500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-3.3219 I-0.3057 J-0.9146
G03 Y8.1649 Z-3.5000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-3.6781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-3.7500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-3.8219 I-0.3057 J-0.9146
G03 Y8.1649 Z-4.0000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-4.1781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-4.2500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-4.3219 I-0.3057 J-0.9146
G03 Y8.1649 Z-4.5000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-4.6781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-4.7500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-4.8219 I-0.3057 J-0.9146
G03 Y8.1649 Z-5.0000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-5.1781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-5.2500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-5.3219 I-0.3057 J-0.9146
G03 Y8.1649 Z-5.5000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-5.6781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-5.7500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-5.8219 I-0.3057 J-0.9146
G03 Y8.1649 Z-6.0000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-6.1781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-6.2500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-6.3219 I-0.3057 J-0.9146
G03 Y8.1649 Z-6.5000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-6.6781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-6.7500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-6.8219 I-0.3057 J-0.9146
G03 Y8.1649 Z-7.0000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-7.1781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-7.2500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-7.3219 I-0.3057 J-0.9146
G03 Y8.1649 Z-7.5000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-7.6781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-7.7500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-7.8219 I-0.3057 J-0.9146
G03 Y8.1649 Z-8.0000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-8.1781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-8.2500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-8.3219 I-0.3057 J-0.9146
G03 Y8.1649 Z-8.5000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-8.6781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-8.7500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-8.8219 I-0.3057 J-0.9146
G03 Y8.1649 Z-9.0000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-9.1781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-9.2500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-9.3219 I-0.3057 J-0.9146
G03 Y8.1649 Z-9.5000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G02 Y9.8352 Z-9.6781 I0.4822 J0.8351
G02 X9.3057 Y9.9146 Z-9.7500 I0.4822 J-0.8351
G03 X8.5178 Y9.8352 Z-9.8219 I-0.3057 J-0.9146
G03 Y8.1649 Z-10.0000 I0.4822 J-0.8351
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.5178 Y8.1649 I-0.9643 J0.0000
G00 Z5.0000
G00 X0.0000 Y0.0000

M05
```

</td> <td>

```
M03 S27000
G00 Z5.0000

(Nr. 1 Ausschnitt: Ausschnitt 1)
G00 X7.2412 Y6.0728
G00 Z0.5000
G01 Z0.0000 F1000 S27000
G02 X6.242595 Y11.014595 Z-0.500000 I1.7588 J2.9272
G02 X7.3444 Y11.9868 I2.757405 J-2.014595
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X6.242595 Y11.014595 Z-1.000000 I1.7588 J2.9272
G02 X7.3444 Y11.9868 I2.757405 J-2.014595
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X6.242595 Y11.014595 Z-1.500000 I1.7588 J2.9272
G02 X7.3444 Y11.9868 I2.757405 J-2.014595
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X6.242595 Y11.014595 Z-2.000000 I1.7588 J2.9272
G02 X7.3444 Y11.9868 I2.757405 J-2.014595
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X6.242595 Y11.014595 Z-2.500000 I1.7588 J2.9272
G02 X7.3444 Y11.9868 I2.757405 J-2.014595
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X6.242595 Y11.014595 Z-3.000000 I1.7588 J2.9272
G02 X7.3444 Y11.9868 I2.757405 J-2.014595
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X6.242595 Y11.014595 Z-3.500000 I1.7588 J2.9272
G02 X7.3444 Y11.9868 I2.757405 J-2.014595
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X6.242595 Y11.014595 Z-4.000000 I1.7588 J2.9272
G02 X7.3444 Y11.9868 I2.757405 J-2.014595
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X6.242595 Y11.014595 Z-4.500000 I1.7588 J2.9272
G02 X7.3444 Y11.9868 I2.757405 J-2.014595
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X6.242595 Y11.014595 Z-5.000000 I1.7588 J2.9272
G02 X7.3444 Y11.9868 I2.757405 J-2.014595
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X6.242595 Y11.014595 Z-5.500000 I1.7588 J2.9272
G02 X7.3444 Y11.9868 I2.757405 J-2.014595
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X6.242595 Y11.014595 Z-6.000000 I1.7588 J2.9272
G02 X7.3444 Y11.9868 I2.757405 J-2.014595
G02 X12.4144 Y8.9404 I1.6556 J-2.9868
G02 X7.2412 Y6.0728 I-3.4144 J0.0596
G02 X6.242595 Y11.014595 I1.7588 J2.9272
G03 X7.241200 Y6.072800 I2.757405 J-2.014595
G00 Z5.0000

(Nr. 2 Ausschnitt: Ausschnitt 32)
G00 X8.5178 Y8.1649 Z5.0000
G00 Z0.5000
G01 Z0.0000 F1000
G02 Y9.8352 Z-0.178100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-0.356217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-0.500000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-0.678100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-0.856217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-1.000000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-1.178100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-1.356217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-1.500000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-1.678100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-1.856217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-2.000000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-2.178100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-2.356217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-2.500000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-2.678100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-2.856217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-3.000000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-3.178100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-3.356217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-3.500000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-3.678100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-3.856217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-4.000000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-4.178100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-4.356217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-4.500000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-4.678100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-4.856217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-5.000000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-5.178100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-5.356217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-5.500000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-5.678100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-5.856217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-6.000000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-6.178100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-6.356217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-6.500000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-6.678100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-6.856217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-7.000000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-7.178100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-7.356217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-7.500000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-7.678100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-7.856217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-8.000000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-8.178100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-8.356217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-8.500000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-8.678100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-8.856217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-9.000000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-9.178100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-9.356217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-9.500000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 Z-9.678100 I0.4822 J0.8351
G02 X9.9643 Y9.0000 Z-9.856217 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 Z-10.000000 I-0.9643 J0.0000
G02 X8.5178 Y8.1649 I0.115474 J0.957361
G02 Y9.8352 I0.4822 J0.8351
G02 X9.9643 Y9.0000 I0.4822 J-0.8351
G02 X8.884526 Y8.042639 I-0.9643 J0.0000
G03 X9.964300 Y9.000000 I0.115474 J0.957361
G03 X8.517800 Y9.835200 I-0.964300 J0.000100
G03 Y8.164900 I0.482200 J-0.835200
G00 Z5.0000
G00 X0.0000 Y0.0000

M05
```

</td>
</tr>

</table>
