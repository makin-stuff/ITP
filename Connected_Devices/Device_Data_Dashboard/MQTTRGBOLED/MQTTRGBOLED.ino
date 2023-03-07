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

// Add in the OLED stuff
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display hight, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI 11
#define OLED_CLK  10
#define OLED_DC   9
#define OLED_CS   7
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

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

  // DEBUG -- not sure about this stuff
  // SSD1306_SWITCHAPVXX = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);  // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000);  // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  
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

      String redString = String(red);
      String greenString = String(green);
      String blueString = String(blue);
      
      // Format the message as JSON string:
      String message = "{\"red\": rr, \"green\": gg, \"blue\": bb}";
      message.replace("rr", redString);
      message.replace("gg", greenString);
      message.replace("bb", blueString);
      
      // print the body of the message:
      mqttClient.println(message);
      // send the message:
      mqttClient.endMessage();
      // send a serial notification:
      Serial.print("published a message: ");
      Serial.println(message);
      // timestamp this message:
      lastTimeSent = millis();

      // DEBUG: Test the display
      writeRGB(redString, greenString, blueString);
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

void writeRGB(String r, String g, String b) {
  display.clearDisplay();

  display.setTextSize(2); // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0, 0);  // Start at top-left corner
  display.cp437(true);  // Use full 256 char 'Code Page 437' font

//  display.println(F("scroll"));
  display.println("Red: " + r);
  display.println("Green: " + g);
  display.println("Blue: " + b);
//  display.print((r));
//   + r + " Green: " + g + " Blue: " + b

  // You MUST call display() after drawing commands to make them visible on screen!
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want... rather, you can batch up a bunch of 
  // drawing operations and then update the screen all at once by calling
  // display.display(). Theese examples demonstarte both approaches...
  display.display();
  delay(2000);
}

//void testdrawchar(void) {
//  display.clearDisplay();
//
//  display.setTextSize(1); // Normal 1:1 pixel scale
//  display.setTextColor(SSD1306_WHITE);  // Draw white text
//  display.setCursor(0, 0);  // Start at top-left corner
//  display.cp437(true);  // Use full 256 char 'Code Page 437' font
//
//  // Not all the characters will fit on the display. This is normal.
//  // Library will draw what it can and the rest will be clipped.
//  for (int16_t i = 0; i < 256; i++) {
//    if (i == '\n') display.write(' ');
//    else display.write(i);
//  }
//
//  // You MUST call display() after drawing commands to make them visible on screen!
//  // display.display() is NOT necessary after every single drawing command,
//  // unless that's what you want... rather, you can batch up a bunch of 
//  // drawing operations and then update the screen all at once by calling
//  // display.display(). Theese examples demonstarte both approaches...
//  display.display();
//  delay(2000);
//}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();  // Show intial text
  delay(100);

  // Scroll in various diractions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}
