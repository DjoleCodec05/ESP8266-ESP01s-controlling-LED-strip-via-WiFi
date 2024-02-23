#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "KOSTAL";
const char* password = "bubasasa";
ESP8266WebServer server(80);

bool relayState = false; // Tracks the state of the relay

const int relayPin = 2; // Change to the appropriate pin

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Ensure relay is initially off

  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  if (server.args() > 0) {
    String stateArg = server.arg("state");
    if (stateArg == "on") {
      relayState = true;
      digitalWrite(relayPin, HIGH); // Turn on relay
    } else if (stateArg == "off") {
      relayState = false;
      digitalWrite(relayPin, LOW); // Turn off relay
    }
  }
  server.send(200, "text/plain", relayState ? "Relay is ON" : "Relay is OFF");
}
