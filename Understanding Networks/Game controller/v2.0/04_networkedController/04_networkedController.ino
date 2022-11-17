// This code was written in reference to Tom Igoe's WiFi JoyStick Example:
// https://github.com/tigoe/BallDropGame/blob/main/BallDropWifiJoystickClient/BallDropWifiJoystickClient.ino
// And @bevchou's websocket-controller code
// https://gist.github.com/bevchou/1aedf7938a7c4e9e1da9b3281ab188dd

#include <EncoderStepCounter.h>
#include <WiFiNINA.h> // Use this for Arduino Nano 33 IoT boards
#include "arduino_secrets.h"

// Initialize the WiFi client library
WiFiClient client;
const char server[] = "10.23.11.143";

// encoder pins:
// UP/DOWN
const int pin1 = 2;
const int pin2 = 3;
// LEFT/RIGHT
const int pin3 = 8;
const int pin4 = 9;
// Create encoder instance:
// UP/DOWN
EncoderStepCounter encoderUD(pin1, pin2);
// LEFT/RIGHT
EncoderStepCounter encoderLR(pin3, pin4);

// encoder previous position:
int oldPosition1 = 0;
int oldPosition2 = 0;

// Button
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
  Serial.begin(9600);

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
  
  // Initialize encoder
  encoderUD.begin();
  encoderLR.begin();

  // Initialize button
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {

  // Check if button is pressed
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
  
  // if you're not using interrupts, you need this in the loop:
  encoderUD.tick();
  // read encoder position:
  int position1 = encoderUD.getPosition();

  // if there's been a change, print it:
  if (position1 > oldPosition1) {
    Serial.println("up");
    client.print("u");
    oldPosition1 = position1;
  } else if (position1 < oldPosition1) {
    Serial.println("down");
    client.print("d");
    oldPosition1 = position1;
  }

  // if you're not using interrupts, you need this in the loop:
  encoderLR.tick();
  // read encoder position:
  int position2 = encoderLR.getPosition();

  // if there's been a change, print it:
  if (position2 > oldPosition2) {
    Serial.println("right");
    client.print("r");
    oldPosition2 = position2;
  } else if (position2 < oldPosition2) {
    Serial.println("left");
    client.print("l");
    oldPosition2 = position2;
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
