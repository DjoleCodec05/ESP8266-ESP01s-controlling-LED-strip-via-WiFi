#include <ESP8266WiFi.h>

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_AUTH_CODE";
const char* serverIP = "SERVER_IP_ADRESS"; // Change to the IP address of your ESP8266 NodeMCU server
const int serverPort = 80;

const int pushButtonPin = 0; // Push button connected to GPIO0

bool buttonState = false;
bool lastButtonState = false;

void setup() {
  Serial.begin(115200);
  pinMode(pushButtonPin, INPUT_PULLUP); // Set push button pin as input with internal pull-up resistor

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
}

void loop() {
  int reading = digitalRead(pushButtonPin);

  if (reading != lastButtonState) {
    // If the button state has changed
    if (reading == LOW) {
      // If the button is pressed
      delay(50); // Debounce delay
      if (digitalRead(pushButtonPin) == LOW) {
        // Ensure it's a valid button press
        buttonState = !buttonState;
        controlRelay(buttonState); // Toggle relay state
        delay(200); // Delay to debounce button press
      }
    }
    lastButtonState = reading; // Save the last button state
  }
}

void controlRelay(bool state) {
  WiFiClient client;

  if (!client.connect(serverIP, serverPort)) {
    Serial.println("Connection to server failed");
    return;
  }

  // Send HTTP GET request to control the relay on the ESP8266 NodeMCU
  String url = "/?state=";
  url += state ? "on" : "off";
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + serverIP + "\r\n" +
               "Connection: close\r\n\r\n");

  delay(10); // Give the server time to respond
  client.stop();
}
