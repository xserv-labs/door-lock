// Modified from https://ucexperiment.wordpress.com/2016/12/18/yunshan-esp8266-250v-15a-acdc-network-wifi-relay-module/
#include <ESP8266WiFi.h>

// Esp8266 pinouts
#define ESP8266_GPIO2    2  // Blue LED.
#define ESP8266_GPIO4    4  // Relay control.
#define ESP8266_GPIO5    5  // Optocoupler input.
#define LED_PIN          ESP8266_GPIO2
#define RELAY_PIN        ESP8266_GPIO4
// WiFi Definitions.
const char ssid[] = "SSID";
const char pswd[] = "PASS";
const char homePage[] = "<meta content=\"width=device-width,user-scalable=no,initial-scale=1,maximum-scale=1,minimum-scale=1\"name=viewport><title>Gateway of Labs</title><style>body{background:#121212;display:flex;justify-content:center;align-items:center;height:98vh}button{min-height:100px;padding:3.6vw 12vh;border:2px solid #fff;outline:0;cursor:pointer;background:#222;color:#fff;border-radius:10px;font-size:1.2rem}</style><button class=button onclick='fetch(\"./open\")'>Unlock</button>";
WiFiServer server( 80 );
volatile int relayState = 0;      // Relay state.

void setup() {
  initHardware();
  connectWiFi();
  server.begin();
}

void GetClient(WiFiClient client) {
  // Read the first line of the request.
  String req = client.readStringUntil('\r');
  client.flush();

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";

  if (req.indexOf("GET") != -1) {
    if (req.indexOf("open") != -1) {
      s += "door opened!";
      relayState = 1;
      digitalWrite(RELAY_PIN, 1);

      delay(20);

      relayState = 0;
      digitalWrite(RELAY_PIN, 0);
    } else {
      s += homePage;
    }
  } else {
      s = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  }

  client.flush();
  s += "</html>\n";

  // Send the response to the client.
  client.print(s);
  delay(1);
}

void loop() {
  // Check if a client has connected.
  WiFiClient client = server.available();
  if ( client ) {
    GetClient(client);
  }
}

void connectWiFi() {
  byte ledStatus = LOW;
  // Set WiFi mode to station (as opposed to AP or AP_STA).
  WiFi.mode(WIFI_STA);

  // WiFI.begin([ssid], [passkey]) initiates a WiFI connection.
  // to the stated [ssid], using the [passkey] as a WPA, WPA2, or WEP passphrase.
  WiFi.begin(ssid, pswd);

  while (WiFi.status() != WL_CONNECTED) {
    // Blink the LED.
    digitalWrite(LED_PIN, ledStatus); // Write LED high/low.
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
    delay(100);
  }
}

void initHardware() {
  pinMode(RELAY_PIN, OUTPUT);            // Relay control pin.
  pinMode(ESP8266_GPIO5, INPUT_PULLUP); // Input pin.
  pinMode(LED_PIN, OUTPUT);             // ESP8266 module blue LED.
  digitalWrite(RELAY_PIN, 0);           // Set relay control pin low.
}
