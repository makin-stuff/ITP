/*
  MQTT Client sender/receiver
  This sketch demonstrates an MQTT client that connects to a broker, subscribes to a topic,
  and both listens for messages on that topic and sends messages to it, a random number between 0 and 255.
  When the client receives a message, it parses it, and PWMs the built-in LED.
  This sketch uses https://public.cloud.shiftr.io as the MQTT broker, but others will work as well.
  See https://tigoe.github.io/mqtt-examples/#broker-client-settings for connection details. 
Libraries used:
  * http://librarymanager/All#WiFiNINA or
  * http://librarymanager/All#WiFi101 
  * http://librarymanager/All#ArduinoMqttClient
  the arduino_secrets.h file:
  #define SECRET_SSID ""    // network name
  #define SECRET_PASS ""    // network password
  #define SECRET_MQTT_USER "public" // broker username
  #define SECRET_MQTT_PASS "public" // broker password
  created 11 June 2020
  updated 30 Dec 2022
  by Tom Igoe
*/

#include <WiFiNINA.h> // use this for Nano 33 IoT, MKR1010, Uno WiFi
#include <ArduinoMqttClient.h>
#include "arduino_secrets.h"

#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Initialize with specific int time and gain values
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

// initialize WiFi connection:
WiFiClient wifi;
MqttClient mqttClient(wifi);

// details for MQTT client:
char broker[] = "public.cloud.shiftr.io";
int port = 1883;
char topic[] = "conndev/makin-stuff";
char clientID[] = "makinClient";

// last time the client sent a message, in ms:
long lastTimeSent = 0;
// message sending interval: every 30 seconds
int interval = 10 * 1000;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // wait for serial monitor to open:
  if (!Serial) delay(3000);
  pinMode(LED_BUILTIN, OUTPUT);
  // connect to WiFi:
  connectToNetwork();

  // set the credentials for the MQTT client;
  mqttClient.setId(clientID);
  // if needed, login to the broker with a username and password:
  mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);
  
  // Check that we can connect to the TCS34725 color sensor
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while(1);
  }
}

void loop() {
  // if you disconnect from the network, reconnect:
  if (WiFi.status() != WL_CONNECTED) {
    connectToNetwork();
    // skip the rest of the loop until you are connected:
    return;
  }

  // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    Serial.println("attempting to connect to broker");
    connectToBroker();
  }
  // poll for new messages from the broker:
  mqttClient.poll();

  // once every interval, send a message:
  if (millis() - lastTimeSent > interval) {
    // get a random analog reading, divide by 4 to set range to 0-255:
    if (mqttClient.connected()) {
      // start a new message on the topic:
      mqttClient.beginMessage(topic);
      // Let's get the RGB values from our sensor
      float r, g, b, c;
      tcs.getRGB(&r, &g, &b); // This function normalizes the RGB data to the range 0-255

      // Convert the floats to integer values:
      int red = (int)r;
      int green = (int)g;
      int blue = (int)b;
      
      // Format the message as JSON string:
      String message = "{\"red\": rr, \"green\": gg, \"blue\": bb}";
      message.replace("rr", String(red));
      message.replace("gg", String(green));
      message.replace("bb", String(blue));
      
      // print the body of the message:
      mqttClient.println(message);
      // send the message:
      mqttClient.endMessage();
      // send a serial notification:
      Serial.print("published a message: ");
      Serial.println(message);
      // timestamp this message:
      lastTimeSent = millis();
    }
  }
}

boolean connectToBroker() {
  // if the MQTT client is not connected:
  if (!mqttClient.connect(broker, port)) {
    // print out the error message:
    Serial.print("MQTT connection failed. Error no: ");
    Serial.println(mqttClient.connectError());
    
    // Turn LED off if not connected to broker
    digitalWrite(LED_BUILTIN, LOW);
    
    // return that you're not connected:
    return false;
  }

  // Turn LED on if connected to broker
  digitalWrite(LED_BUILTIN, HIGH);
  
  // set the message receive callback:
  mqttClient.onMessage(onMqttMessage);
  // subscribe to a topic:
  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  mqttClient.subscribe(topic);

  // once you're connected, you return that you're connected:
  return true;
}

void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.println("Received a message with topic ");
  Serial.print(mqttClient.messageTopic());
  Serial.print(", length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");
  String incoming = "";
  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    incoming += (char)mqttClient.read();
  }
  // convert the incoming string to an int so you can use it:
  int result = incoming.toInt();
  // print the result:
  Serial.println(result);
}

void connectToNetwork() {
  // try to connect to the network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to: " + String(SECRET_SSID));
    // Connect to WPA / WPA2 network:
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }

  // print IP address once connected:
  Serial.print("Connected. My IP address: ");
  Serial.println(WiFi.localIP());
}
