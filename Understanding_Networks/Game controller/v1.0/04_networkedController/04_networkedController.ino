// This code was written in reference to Tom Igoe's WiFi JoyStick Example:
// https://github.com/tigoe/BallDropGame/blob/main/BallDropWifiJoystickClient/BallDropWifiJoystickClient.ino
// And @bevchou's websocket-controller code
// https://gist.github.com/bevchou/1aedf7938a7c4e9e1da9b3281ab188dd

#include <Encoder.h>
#include <WiFiNINA.h> // Use this for Arduino Nano 33 IoT boards
#include "arduino_secrets.h"

// Initialize the WiFi client library
WiFiClient client;
const char server[] = "10.18.227.179";

// Up and down movement
// Yellow knob
Encoder udEnc(2, 3);
long udPrev = -999;
long udNew;

// Left and right movement
// Blue knob
Encoder lrEnc(8, 9);
long lrPrev = -999;
long lrNew;

// Button to login
// For now, I'm going to use the button built into the blue encoder
// Because I can't fit another button onto the small bread board
int buttonPin = 21;
int buttonStatePrev = HIGH;
int buttonState;
// Add some button debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// WiFi stuff
const int sendInterval = 50;  // minimum time between messages to the server
long lastTimeSent = 0;  // timestamp of the last server message

void setup() {
  Serial.begin(115200);
  // If serial monitor is not open, wait three seconds
  if (!Serial) delay(3000);
  
  Serial.println("Networked Game Controller for Ball Drop Game");

  // Attempt to connect to the WiFi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    // Connect to WPA/WPA2 network.
    WiFi.begin(SECRET_SSID, SECRET_PASS);

    // Wait three seconds for connection
    delay(3000);
  }

  // You're connected now, so print out the status
  printWiFiStatus();

  // Initialize digital pin for button
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Check if the button is pressed
  int reading = digitalRead(buttonPin);
  if (reading != buttonStatePrev) {
    lastDebounceTime = millis();
  }

  // Software debouncing
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        Serial.println("Button pressed");
        
        // If the client's not connected, let's connect
        if (!client.connected()) {
          Serial.println("Connecting...");
          client.connect(server, 8080);
        } else {  // else disconnect
          Serial.println("Disconnecting...");
          client.print("x");
          client.stop();
        }
      }
    }
  }
  buttonStatePrev = reading;
  
  // First, check up/down encoder
  udNew = udEnc.read();
  // If the position has changed
  if (udNew != udPrev) {
    // Check which direction we moved
    if (udNew > udPrev && udNew % 4 == 0) {
      Serial.println("up");
      client.print("u");
    } else if (udNew < udPrev && udNew % 4 == 0){
      Serial.println("down");
      client.print("d");
    }
    udPrev = udNew;
  }

  // Next, check left/right encoder
  lrNew = lrEnc.read();
  // If the position has changed
  if (lrNew != lrPrev) {
    // Check which direction we moved
    if (lrNew > lrPrev && lrNew % 4 == 0) {
      Serial.println("right");
      client.print("r");
    } else if (lrNew < lrPrev && lrNew % 4 == 0){
      Serial.println("left");
      client.print("l");
    }
    lrPrev = lrNew;
  }

  // Turn on the built-in LED based on the connection state
  digitalWrite(LED_BUILTIN, client.connected());

  // If there's incoming data from the client, print it serially
  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
}

void printWiFiStatus() {
  // Print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP address: ");
  Serial.println(ip);

  // Print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}
