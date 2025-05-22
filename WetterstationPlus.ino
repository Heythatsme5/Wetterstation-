/*Wetterstation Plus
  Nachname, Vorname
  Klassenbezeichnung
  
  Verwendete Bibliotheken:
  Adafruit SSD1306 2.5.13
  DHT sensor library 1.4.6
  Adafruit Unified Sensor 1.1.14
   */
#include <Adafruit_SSD1306.h> // Bibliothek für das OLED-Display
#include <DHT.h> // Bibliothek für den DHT-Sensor (Temperatur und Luftfeuchtigkeit)
#include <ESP8266WiFi.h> // Bibliothek für WLAN-Funktionalität des ESP8266
#include <ESP8266WebServer.h>
#include "index_html.h"
#include "style_css.h"

// OLED-Konfiguration
#define SCREEN_WIDTH 128 // Breite des OLED-Displays in Pixel
#define SCREEN_HEIGHT 64 // Höhe des OLED-Displays in Pixel
#define OLED_RESET -1 // Kein Reset-Pin verwendet
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // OLED-Objekt erstellen


// DHT-Konfiguration
#define DHT_TYPE DHT11  // Sensor-Typ ist DHT11
#define DHT_PIN D5 // Datenleitung des DHT-Sensors an Pin D5
#define DHT_POWER D0 // Stromversorgungspin
DHT dht(DHT_PIN, DHT_TYPE); // DHT-Sensor initialisieren

#define LOGO_HEIGHT   16 // Höhe der Icons
#define LOGO_WIDTH    16  // Breite der Icons

// Sonstiges
const int sensor = 0;  // A0 Pin für Helligkeitssensor
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 2000;
unsigned long lastSwitch = 0; // Letzter Umschaltzeitpunkt
int screenIndex = 0; // Aktueller Bildschirmindex (0, 1 oder 2)

unsigned long screenDelay = 20000; // Zeit bis zum nächsten Bildschirmwechsel (20 Sekunden)

int adcWert; // Variable für den gemessenen Lichtwert
float h, t, tautemp,balltemp;

//Platzhalter für Übersichtlichkeit
void process_Index(); 
void handleNotFound();
void handleData();
void handleCSS();

ESP8266WebServer server(80);        // server port

const unsigned char sun_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00100000, 0b10000010, //   #     #     # 
  0b00010000, 0b10000100, //    #    #    #  
  0b00001000, 0b00001000, //     #       #   
  0b00000001, 0b11000000, //        ###      
  0b00000111, 0b11110000, //      #######    
  0b00000111, 0b11110000, //      #######    
  0b00001111, 0b11111000, //     #########   
  0b01101111, 0b11111011, //  ## ######### ##
  0b00001111, 0b11111000, //     #########   
  0b00000111, 0b11110000, //      #######    
  0b00000111, 0b11110000, //      #######    
  0b00010001, 0b11000100, //    #   ###   #  
  0b00100000, 0b00000010, //   #           # 
  0b01000000, 0b10000001, //  #      #      #
  0b00000000, 0b10000000, //         #       
};


const unsigned char bulb_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000011, 0b11100000, //       #####     
  0b00000100, 0b00010000, //      #     #    
  0b00001000, 0b00001000, //     #       #   
  0b00010000, 0b00000100, //    #         #  
  0b00010000, 0b00000100, //    #         #  
  0b00010000, 0b00000100, //    #         #  
  0b00010000, 0b00000100, //    #         #  
  0b00010000, 0b00000100, //    #         #  
  0b00001000, 0b00001000, //     #       #   
  0b00000100, 0b00010000, //      #     #    
  0b00000011, 0b11100000, //       #####     
  0b00000010, 0b00100000, //       #   #     
  0b00000011, 0b11100000, //       #####     
  0b00000010, 0b00100000, //       #   #     
  0b00000011, 0b11100000, //       #####     
};

const unsigned char humidity2_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000001, 0b10000000, //        ##       
  0b00000011, 0b11000000, //       ####      
  0b00000111, 0b11100000, //      ######     
  0b00001111, 0b11110000, //     ########    
  0b00001111, 0b11110000, //     ########    
  0b00011111, 0b11111000, //    ##########   
  0b00011111, 0b11011000, //    ####### ##   
  0b00111111, 0b10011100, //   #######  ###  
  0b00111111, 0b10011100, //   #######  ###  
  0b00111111, 0b00011100, //   ######   ###  
  0b00011110, 0b00111000, //    ####   ###   
  0b00011111, 0b11111000, //    ##########   
  0b00001111, 0b11110000, //     ########    
  0b00000011, 0b11000000, //       ####      
  0b00000000, 0b00000000, //                 
};

const unsigned char temperature_icon16x16[] =
{
  0b00000001, 0b11000000, //        ###      
  0b00000011, 0b11100000, //       #####     
  0b00000111, 0b00100000, //      ###  #     
  0b00000111, 0b11100000, //      ######     
  0b00000111, 0b00100000, //      ###  #     
  0b00000111, 0b11100000, //      ######     
  0b00000111, 0b00100000, //      ###  #     
  0b00000111, 0b11100000, //      ######     
  0b00000111, 0b00100000, //      ###  #     
  0b00001111, 0b11110000, //     ########    
  0b00011111, 0b11111000, //    ##########   
  0b00011111, 0b11111000, //    ##########   
  0b00011111, 0b11111000, //    ##########   
  0b00011111, 0b11111000, //    ##########   
  0b00001111, 0b11110000, //     ########    
  0b00000111, 0b11100000, //      ######     
};

const unsigned char DPT_icon16x16[] = {
    
    0b00000011, 0b10000000, //       ###       
    0b00000110, 0b10000000, //      ## #       
    0b00000101, 0b11000000, //      # ###      
    0b00000101, 0b11000000, //      # ###      
    0b00000101, 0b11001000, //      # ###  #   
    0b00000101, 0b11011100, //      # ### ###  
    0b00000101, 0b11011110, //      # ### #### 
    0b00000101, 0b11111110, //      # ######## 
    0b00000101, 0b11111111, //      # #########
    0b00000101, 0b11011111, //      # ### #####
    0b00001011, 0b11101111, //     # ##### ####
    0b00001111, 0b11101110, //     ####### ### 
    0b00011111, 0b11101100, //    ######## ##  
    0b00001111, 0b10100000, //     ##### #     
    0b00001111, 0b01100000, //     #### ##     
    0b00000111, 0b11000000  //      #####      
};

// Funktion zur zentrierten Textausgabe
void drawCentered(const String& text, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h); // Textgröße berechnen
  int x = (SCREEN_WIDTH - w) / 2; // X-Position zum Zentrieren
  display.setCursor(x, y);
  display.print(text);
}

void setup() {
  WiFi.mode(WIFI_AP); // ESP8266 im Access Point Modus
  WiFi.softAP("myOwnWiFi", "a1b2c3d4e5!"); // Netzwerkname + Passwort
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // OLED initialisieren (Adresse: 0x3C)
  dht.begin(); // DHT Sensor starten
  Serial.begin(115200); // Serielle Kommunikation starten
  delay(500);

  // set event handler
  server.on("/", process_Index);
  server.onNotFound(handleNotFound);
  server.on("/data", handleData);
  server.on("/style.css", handleCSS);
  
  server.begin();       // start server
  Serial.println("HTTP Server started");
  

  // Banner nur beim Start
  display.clearDisplay();
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  drawCentered("WETTERSTATION+", 16);
  display.drawLine(0, 36, SCREEN_WIDTH, 36, SSD1306_WHITE);
  drawCentered("NACHNAME, VORNAME", 42);
  drawCentered("KLASSENBEZEICHNUNG", 52);
  display.display();
  delay(10000);
  display.clearDisplay();
  display.display();
  lastSwitch = millis();
}

void loop() {
   server.handleClient();
if (millis() - lastUpdate < updateInterval) 
{
  return;
}


   
  h = dht.readHumidity(); // Luftfeuchte lesen
  t = dht.readTemperature(); // Temperatur lesen
  tautemp; // Taupunkt
  balltemp; // Feuchtkugeltemperatur
  adcWert = analogRead(sensor); // Lichtwert lesen

// Taupunkttemperatur berechnen
//Konstanten k2,k3 sind abhängig vom Aggregatzustand
  if (t >= 0) { //Wenn Temperatur über oder gleich 0 °C dann:
    float k2 = 17.62, k3 = 243.12;
    tautemp = (k3 * ((k2 * t) / (k3 + t) + log(h / 100))) /
              ((k2 * k3) / (k3 + t) - log(h / 100));
              //Mittels Logarithmus wird umgerechnet von relativer Luftfeuchte in den Maßstab wie nah die Luft an der Sättigung ist
  } else { //Ansonsten ... weil Temperatur unter 0 °C
    //frostige Bedingungen
    float k2 = 22.46, k3 = 272.62;
    tautemp = (k3 * ((k2 * t) / (k3 + t) + log(h / 100))) /
              ((k2 * k3) / (k3 + t) - log(h / 100));
              //Mit (k3*((k2*t)/(k3+t) rechnet man den temperaturabhängigen Teil des Sättigungsdampdrucks
  }
// Feuchtkugeltemperatur berechnen
  balltemp = t * atan(0.151977 * sqrt(h + 8.313659)) + atan(t + h) - atan(h - 1.676331) +
             0.00391838 * pow(h, 1.5) * atan(0.023101 * h) - 4.686035;
//Atan sorgt dafür dass die Ergebnisse "realistisch" bleiben
//0.151977 ist ein Gewichtungsfaktor
// pow berchnet die Potenz: h hoch 3/2
//sqrt berechnet die Wurzel aus der Luftfeuchtigkeit


  // Bildschirmumschaltung alle 20 Sekunden
  if (millis() - lastSwitch >= screenDelay) {
    screenIndex = (screenIndex + 1) % 3;
    lastSwitch = millis();
  }

  display.clearDisplay(); // Display löschen
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

 // ---------- SERIELLE AUSGABE ----------
  Serial.println("---------------------");
  Serial.println("Luftfeuchtigkeit: " + (String)h + " %");
  Serial.println("Temperatur: " + (String)t + " °C");
  Serial.println("Taupunkttemperatur: " + (String)tautemp + " °C");
  Serial.println("Feuchtkugeltemp: " + (String)balltemp + " °C");
  Serial.println("Helligkeit: " + (String)adcWert);
  Serial.println("---------------------");

// Erster Bildschirm: einfache Textanzeige
  if (screenIndex == 0) {
    display.println(">----SENSORDATEN----<");
    display.println("rF: " + String(h) + " %");
    display.println("Temp: " + String(t) + " " + (char)247 + "C");
    display.println("DPT: " + String(tautemp) + " " + (char)247 + "C");
    display.println("WBT: " + String(balltemp) + " " + (char)247 + "C");
    display.println("Licht: " + String(adcWert));
    display.setCursor(0, 60); //Display-Ende
  display.println("---------------------");
   // Zweiter Bildschirm: Symbole mit Werten
  } else if (screenIndex == 1) {
   display.println(">----SENSORDATEN----<"); //21 Character
  //relative Humidity
  display.drawBitmap(0,8, humidity2_icon16x16, LOGO_WIDTH, LOGO_HEIGHT, 1); 
  display.setCursor(0,15); 
  display.println("   " + (String)h + "% " ); 

  //Temperature
  display.drawBitmap(64,8, temperature_icon16x16, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.setCursor(64,15);//Temperature
  display.println("   " + (String)t + "" + (char)247 +"C");
  
  //Dew Point Temperature
  display.drawBitmap(0,29, DPT_icon16x16, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.setCursor(0,35);
  display.println("   " + (String)tautemp + "" + (char)247 +"C");

  //Wet Bulb Temperature
  display.drawBitmap(64,29,bulb_icon16x16, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.setCursor(64, 35);
  display.println("   " + (String)balltemp + "" + (char)247 +"C"); //Wet Bulb Temperature

  //Light
  display.drawBitmap(0,48,sun_icon16x16, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.setCursor(0, 56);
  display.println("   " + (String)adcWert); //Licht
  // Dritter Bildschirm: Netzwerkdaten
  } else if (screenIndex == 2) {
    display.println(">---NETZWERKDATEN---<");
    display.print("IP:");
    display.println(WiFi.softAPIP());
    display.print("MAC:");
    display.println(WiFi.macAddress());
    display.setCursor(0, 60); //Display-Ende
    display.println("---------------------");
  }
  display.display(); // Inhalte auf OLED anzeigen
  lastUpdate = millis();
}
void process_Index()
{ 
   Serial.println("send index.html");
    server.send(200, "text/html", indexHTML);          // send index.html ("index_html.h")
}
void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found");    // send HTTP status 404 (Not Found)
}
void handleData()
{
    server.sendHeader("Cache-Control", "no-cache");
    server.send(200, "text/plain",String(adcWert) + "," +String(h) + "," + String(t) + "," + String(tautemp) + "," + String(balltemp) + "," + WiFi.softAPIP().toString() + "," + String(WiFi.macAddress()));
}
void handleCSS()
{
   Serial.println("send style.css");
server.send(200, "text/css", styleCSS);
}
