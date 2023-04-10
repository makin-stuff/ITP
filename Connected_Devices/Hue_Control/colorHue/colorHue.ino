#include <Arduino_LSM6DS3.h>
#include <SPI.h>
#include <WiFiNINA.h>  // For Nano 33 IoT
#include <ArduinoHttpClient.h>
#include "arduino_secrets.h"

int status = WL_IDLE_STATUS;  // The Wifi radio's status
char hueHubIP[] = "172.22.151.226"; // IP address of the HUE bridge
String hueUserName = "dKXLEBprVuuxDjDqMU4l6KlqLcA2P19YJQOS5oaT";  // hue bridge username

// make a wifi instance and a HttpClient instance:
WiFiClient wifi;
HttpClient httpClient = HttpClient(wifi, hueHubIP);
// change the values of these two in the arduino_secrets.h file:
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

void setup() {
  Serial.begin(9600);
  if (!Serial) delay(3000); // wait for serial port to connect

  pinMode(LED_BUILTIN, OUTPUT);

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    digitalWrite(LED_BUILTIN, LOW);
  }

  // You're connected now, so print out the data:
  Serial.print("You're connected to the network IP = ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  digitalWrite(LED_BUILTIN, HIGH);
  
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while(1);
  }
}

void loop() {
//  digitalWrite(LED_BUILTIN, HIGH);
  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    // On/off switch
    if (z > 0.9) {
      Serial.println("Pointing up");
      sendRequest(3, "on", "true"); // turn light on
    } else if (z < -0.9) {
      Serial.println("Pointing down");
      sendRequest(3, "on", "false");  // turn light off
    } 
    // This is for color changing!
    else if (z > -0.3 && z < 0.3) {
      // For debugging
      Serial.println("IN THE HUE CHANGE");
      
      // First, let's make sure the light is on
      sendRequest(3, "on", "true"); // turn light on
      if (x > -0.1 && x < 0.1) {
        if (y > -1.1 && y < -0.9) {
          Serial.println("1");
          sendRequest(3, "hue", "10000");
        }
      }
      if (x > 0.3 && x < 0.8) {
        if (y > -0.8 && y < -0.3) {
          Serial.println("2");
          sendRequest(3, "hue", "20000");
        }
      }
      if (x > 0.3 && x < 0.8) {
        if (y > 0.3 && y < 0.8) {
          Serial.println("3");
          sendRequest(3, "hue", "30000");
        }
      }
      if (x > -0.1 && x < 0.1) {
        if (y > 0.9 && y < 1.1) {
          Serial.println("4");
          sendRequest(3, "hue", "40000");
        }
      }
      if (x > -0.8 && x < -0.3) {
        if (y > 0.3 && y < 0.8) {
          Serial.println("5");
          sendRequest(3, "hue", "50000");
        }
      }
      if (x > -0.8 && x < -0.3) {
        if (y > -0.8 && y < -0.3) {
          Serial.println("6");
          sendRequest(3, "hue", "60000");
        }
      }
    }
  }

  delay(1000);
}

void sendRequest(int light, String cmd, String value) {
  // Make a String for the HTTP request path:
  String request = "/api/" + hueUserName;
  request += "/lights/";
  request += light;
  request += "/state/";

  String contentType = "application/json";

  // make a string for the JSON command:
  String hueCmd = "{\"" + cmd;
  hueCmd += "\":";
  hueCmd += value;
  hueCmd += "}";
  // see what you assembled to send:
  Serial.print("PUT request to server: ");
  Serial.println(request);
  Serial.print("JSON command to server: ");

  // make the PUT request to the hub:
  httpClient.put(request, contentType, hueCmd);

  // read the status code and body of the response
  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();

  Serial.println(hueCmd);
  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  Serial.print("Server response: ");
  Serial.println(response);
  Serial.println();
}
