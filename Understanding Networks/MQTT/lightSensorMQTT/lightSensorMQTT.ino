/*
  MQTT Client sender

  This sketch demonstrates an MQTT client that connects to a broker, subsrcibes to a topic,
  and both listens for messages on that topic and sends messages to it, a random number between 0 and 15.
  When the client receives a message, it parses it, and if the number matches the client's
  number (myNumber, chosen arbitrarily), it sets an LED to full. When nothing is happening,
  if the LED is not off, it's faded down one point every time through the loop.

  This sketch uses https://shiftr.io/try as the MQTT broker.

  the arduino_secrets.h file:
  #define SECRET_SSID ""    // network name
  #define SECRET_PASS ""    // network password
  #define SECRET_MQTT_USER "public" // broker username
  #define SECRET_MQTT_PASS "public" // broker password

  created 11 June 2020
  updated 11 April 2021
  by Tom Igoe
*/

#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include "arduino_secrets.h"

#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

// initialize WiFi connection:
WiFiClient wifi;
MqttClient mqttClient(wifi);

// details for MQTT client:
char broker[] = "test.mosquitto.org";
int port = 1883;
char topic[] = "undnet/makin-stuff";
char clientID[] = "makinClient";

// last time the client sent a message, in ms:
long lastTimeSent = 0;
// message sending interval: every 5 minutes
//int interval = 60 * 1000;
long interval = 1000*60;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // wait for serial monitor to open:
  while (!Serial) delay(3000);

  // initialize WiFi, if not connected:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(SECRET_SSID);
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }
  // print IP address once connected:
  Serial.print("Connected. My IP address: ");
  Serial.println(WiFi.localIP());

  // set the credentials for the MQTT client:
  mqttClient.setId(clientID);
  // login to the broker with a username and password:
  //mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);

  // try to connect to the MQTT broker once you're connected to WiFi:
  while (!connectToBroker()) {
    Serial.println("attempting to connect to broker");
    delay(1000);
  }
  Serial.println("connected to broker");

  // Check that we can connect to the TCS34725 color sensor
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop() {
  // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    Serial.println("reconnecting");
    connectToBroker();
  }

  // once every interval, send a message:
  if (millis() - lastTimeSent > interval) {
    // start a new message on the topic:
    mqttClient.beginMessage(topic);
    // Let's the color temperature and lux measurement from our sensor
    uint16_t r, g, b, c, colorTemp, lux;
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
    lux = tcs.calculateLux(r, g, b);

    // Now, put our measurements into a JSON string
    String body = "{\"ColorTemp\": tt, \"Lux\": ll}";
    body.replace("tt", String(colorTemp));
    body.replace("ll", String(lux));

    // For debugging
    Serial.println(body);
    
    // Print the body of the messafe:
    mqttClient.println(body);
    // send the message:
    mqttClient.endMessage();
    lastTimeSent = millis();
  }
}

boolean connectToBroker() {
  // if the MQTT client is not connected:
  if (!mqttClient.connect(broker, port)) {
    // print out the error message:
    Serial.print("MQTT connection failed. Error no: ");
    Serial.println(mqttClient.connectError());
    // return that you're not connected:
    return false;
  }
  // once you're connected, you
  // return that you're connected:
  return true;
}
