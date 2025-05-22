# Wetterstation+
Webbasierte Wetterstation mit dem DHT11 &amp; ESP8266

Dieses Projekt realisiert eine webbasierte Wetterstation auf Basis eines ESP8266-Mikrocontrollers und einem DHT11-Sensor zur Messung von Temperatur und Luftfeuchtigkeit. 
Ergänzt wird die Station um einen Helligkeitssensor an A0, ein OLED-Display zur lokalen Anzeige und eine responsive Webseite zur Auswertung der Messdaten.


Projektaufgabe

Ziel des Projekts ist es, eine voll funktionsfähige, cyber-physische Wetterstation zu entwickeln, die:

meteorologische Messgrößen (Temperatur, relative Luftfeuchtigkeit, Helligkeit) automatisch erfasst,

die Rohdaten über serielle Schnittstelle und ein OLED-Display (SSD1306, 128×64) sowohl textuell als auch grafisch anzeigt,

Taupunkt- und Feuchtkugeltemperatur berechnet und darstellt,

einen WLAN-Access-Point bereitstellt, unter dem eine selbst entwickelte, responsive Webseite per AJAX die aktuellen Messwerte ausgibt,

Planungs- und Dokumentationsschritte (Netzplan, Ablaufplan, schriftliche Dokumentation) strukturiert umsetzt.


Warum ist das Projekt sinnvoll?

Praxisnahe Verknüpfung von Hardware und Software: Durch die Integration eines Sensors, Mikrocontrollers und Webtechnologien werden Kenntnisse in den Bereichen Embedded Systems, Sensorik, WLAN-Kommunikation und Webentwicklung geschult.

Datenanalyse und Visualisierung: Die Berechnung klimatischer Kennwerte (Taupunkt, Feuchtkugeltemperatur) und deren grafische Aufbereitung auf Display und Webseite fördern das Verständnis für meteorologische Zusammenhänge und Datenaufbereitung.

Ressourcenmanagement auf Mikrocontroller-Ebene: Die Beschränkungen von Speicherplatz und Rechenleistung des ESP8266 erfordern effiziente Programmier- und Speicherstrategien (z. B. Bitmap-Konvertierung, asynchrones AJAX, Vermeidung langer delay()-Blocks).

Full-Stack-Entwicklung: Die Kombination aus Embedded-Code auf dem ESP8266, lokaler Display-Ansteuerung und Frontend-Entwicklung (HTML, CSS, AJAX) entspricht realen Anforderungen in Industrie und Forschung.

Selbstständiges Projektmanagement: Die Planung mittels Netz- und Ablaufplan, die Gliederung in Arbeitspakete und die schriftliche Dokumentation stärken methodische Kompetenzen im Projektmanagement.


Hinweise zur Hardware

Die verwendeten Pins für den DHT11-Sensor und das OLED-Display können je nach verwendeter Hardware variieren. Prüfe daher vor dem Aufbau die korrekte Pinbelegung und passe den Code entsprechend an.

Je nach OLED-Modell kann die I2C-Adresse des Displays entweder 0x3C oder 0x3D sein. Stelle sicher, dass du die richtige Adresse im Code einträgst, um eine erfolgreiche Kommunikation zu gewährleisten.
