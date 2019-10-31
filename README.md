# RFID Schlusselkasten - WIP

RFID Schlüsselkasten für die Fachschaft Informatik der Uni Tübingen

Basierend auf:
- NodeMCU-esp8266 (v.2)
- RFID-RC522
- 4-Digit 7 Segment Display (TM1637)
- ULN2003APG Steppercontroller

# Done
- Wifi-Anbindung
- API
- HTTPS-GET der erlaubten Studierendenausweise über API mit Überprüfung des
  SSL-Zertifikats
- Auslesen der Studierendenausweise über RFID-RC522
- Erkennung Tür offen/geschlossen
- Steppersteuerung
- 4 Digit (7 Segment)-Display für Status-Codes
- Fallbacks

# Todo
- ~~USV (UPS) Interne Powerbank~~ (aufgegeben auf Grund von Platzmangel)
- Speicherung der erlaubten UID auf SPIFFS / SD-Card
- regelmäßiges Fetchen der erlaubten UIDs
- Loggen von Aktionen

# Setup
## Server
- api.php in public_html Ordner kopieren
- user.csv eine Ebene höher erstellen
- UIDs im Format: UID(lowercase);Name eintragen

Bsp.:<br>
12bb4f00;MaxMuster<br>
bb420975;ErikaMuster<br>
89e370ae;JaneDoe


## NodeMCU
- ChangeMe Daten im sketch.ino anpassen und hochladen
- Hardware zusammenlöten, Verbauen, Fertig

## Verkabelung
Die Verkabelung ist wie folgt:

<img src="/Schematics.png" alt="Wiring schematics" height="400px">


# Status-Code
Der Schlüsselkasten zeigt permanent einen Status-Code an. Diese haben folgende bedeutungen.

- 1XXX Bootup
  - 11XX Wifi 
      - Connecting 11nn <1140 (nn gibt die Anzahl der Verbindungsversuche an)
  - 12XX Fetching
      - 12nn <1220 (nn gibt Anzahl der HTTPS-Verbindungsversuche an)
      - 1250 Header Erhalten
      - 1260 UIDs auslesen
- 2XXX Activ
  - 2001 Auf Karte wartend
  - 2002 Karte gelesen
  - 2003 Warten bis Kasten geschlossen
- 3XXX Control
  - 3001 Karte erlaubt
  - 3002 Karte nicht erlaubt
  - 3003 Öffnen des Schlosses
  - 3004 Schließen des Schlosses
  - 3005 Debug 30 Steps
- 4XXX Error
  - (4000 Died in boot)
  - 4001 Died while connecting with Wifi
  - 4002 Died while fetching
  - (4003  Died in Main Loop)

# License
```
Schlüsselkasten for the Computer-Science Student Union of the Univertity of Tuebingen
Copyright (C) 2019  Jules Kreuer / not-a-feature
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
````
See [LICENSE](LICENSE) for more details.

