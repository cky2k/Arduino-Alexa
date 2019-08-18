#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#define SERIAL_BAUDRATE                 115200
fauxmoESP fauxmo;
 
// -----------------------------------------------------------------------------
// Wifi defines
// -----------------------------------------------------------------------------

//#define WIFI_SSID "Router SSID"
//#define WIFI_PASS "Router Password"

// -----------------------------------------------------------------------------
// PIN definitions
// -----------------------------------------------------------------------------

#define SWITCH_PIN1 13
#define SWITCH_PIN2 15
 
void wifiSetup() {
  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);
 
  // Connect
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
  }
  Serial.println();
 
  // Connected!
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}
 
void setup() {

// -----------------------------------------------------------------------------
// Configure and set PINs
// -----------------------------------------------------------------------------

  delay(10000);
  
  pinMode(SWITCH_PIN1, OUTPUT);
  digitalWrite(SWITCH_PIN1, LOW);

  pinMode(SWITCH_PIN2, OUTPUT);
  digitalWrite(SWITCH_PIN2, LOW);
  
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();
    Serial.println("FauxMo Setup");
    Serial.println("Nach der Verbindung sage: Alexa schalte Bürolicht1 / Bürolicht2 an / aus .");
 
  // Wifi
  wifiSetup();
  
// -----------------------------------------------------------------------------
// Define devices
// -----------------------------------------------------------------------------

  // Fauxmo
  fauxmo.addDevice("Bürolicht1");
  fauxmo.addDevice("Bürolicht2");

  fauxmo.setPort(80);
  fauxmo.enable(true);

   fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) 
   {
      Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
      
      // -----------------------------------------------------------------------------
      // Define queries
      // -----------------------------------------------------------------------------
 
      if ( (strcmp(device_name, "Bürolicht1") == 0) ) {
        if (state) {
          digitalWrite(SWITCH_PIN1, HIGH);
          Serial.printf("Bürolicht1 wird angeschaltet.");
        } else {
          digitalWrite(SWITCH_PIN1, LOW);
          Serial.printf("Bürolicht1 wird ausgeschaltet.");
        }
      }


      if ( (strcmp(device_name, "Bürolicht2") == 0) ) {
        if (state) {
          digitalWrite(SWITCH_PIN2, HIGH);
          Serial.printf("Bürolicht2 wird angeschaltet.");
        } else {
          digitalWrite(SWITCH_PIN2, LOW);
          Serial.printf("Bürolicht2 wird angeschaltet.");
        }
      }
       }
       );
}
 
void loop() {
  fauxmo.handle();
}
