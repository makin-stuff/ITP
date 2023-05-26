/*
  Created May 2, 2023
  by Priyanka Makin

  Based on the DweetGet example by Tom Igoe.

  This code creates an HTTP client and connected to the OpenWeather Map API.
  It gets the current AQI data every few minutes and a watch dog timer is implemented
  in the case the requests get hung up on an error.
*/

#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <wdt_samd21.h>
#include <Adafruit_NeoPixel.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const char serverAddress[] = "api.openweathermap.org";  // server address
int port = 80;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

int errCount = 0; // Holds the number of sequential errors
int loopCount = 0;  // Counts the number of times we go through the ~10 second loop
int currAqi;

// For the neopixel ring
int LEDpin = 6;
int numPixels = 16;
Adafruit_NeoPixel pixels(numPixels, LEDpin, NEO_GRB + NEO_KHZ800);
uint32_t currColor;
uint32_t white;
uint32_t green;
uint32_t yellow;
uint32_t orange;
uint32_t red;

// assemble the path for the GET message:
String path = "/data/2.5/air_pollution?lat=33.4&lon=111.8&appid=5070c0ca8af79010d5d00b6f33016df8";

void setup() {
  Serial.begin(9600);
  if (!Serial) delay(3000);

  // Initialize the LED ring
  pixels.begin();
  Serial.println("Clear");
  pixels.clear();
  pixels.show();
  delay(3000);
  
  uint32_t purple = pixels.Color(153, 51, 255);
  uint32_t blue = pixels.Color(0, 0, 255);

  // For air quality status
  white = pixels.Color(255, 255, 255);
  green = pixels.Color(0, 204, 0);
  yellow = pixels.Color(255, 255, 0);
  orange = pixels.Color(255, 102, 0);
  red = pixels.Color(255, 0, 0);

  // Turn the pixels purple while waiting to connect
  pixels.fill(purple);
  pixels.show();
  
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);     // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // Once Arduino connected to the internet, turn the pixels blue
  pixels.fill(blue);
  pixels.show();
  
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Configure the pins for needed to drive the pumps
  pinMode(2, OUTPUT); // pump 1
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT); // pump2
  pinMode(5, OUTPUT);

  wdt_init(WDT_CONFIG_PER_16K); // Define the "timeout" of the watch dog timer. 16K = 16384 clock cycles (16 sec)
}

void loop() {

  // We want to update the air quality data every 20 times through this loop
  // As of right now, I'm not exactly sure how many seconds that is...
  if (loopCount % 20 == 0) {
    currAqi = getAQI();
    Serial.print("AQI: ");
    Serial.println(currAqi);
    updateLEDs(currAqi);
    breatheLungs(currAqi);
    // Reset the loop count so that the variable doesn't overflow
    loopCount = 0;
  }

  // Do one heart beat - takes approx 6 seconds
  heartBeat(currColor);
  
  // Reset the watchdog timer
  Serial.println("I'm resetting the watchdog");
  wdt_reset();
  loopCount++;
  Serial.println(loopCount);
}

int getAQI() {
    // send the GET request
  Serial.println("making GET request");
  client.get(path);

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  client.stop();  // close the request

  // Increment error code if it's appropriate
  if (statusCode == -2 | statusCode == -3) {
    digitalWrite(LED_BUILTIN, HIGH);
    errCount++;
    Serial.println(errCount);
  } else if (statusCode == 200) {
    digitalWrite(LED_BUILTIN, LOW);
    errCount = 0;
  }
  
  // If an error count more than 3
  if (errCount > 2) {
    Serial.println("I've had too many errors");
    while(1);  // Wait so that the watch dog can reset
  }
  /*
    Typical response is:
    {"coord":{"lon":111.8,"lat":33.4},
    "list":[{"main":{"aqi":3},
      "components":{"co":420.57,"no":0,"no2":4.24,"o3":84.4,"so2":9.54,"pm2_5":39.52,"pm10":40.9,"nh3":0.98},
      "dt":1683054449}]}

    I want "aqi": numberValue
  */
  // now parse the response looking for "aqi":
  int labelStart = response.indexOf("aqi\":");
  // The AQI value is right after the "aqi": substring
  // That's 5 characters after the initial "
  int contentStart = labelStart + 5;
  // find the following } and get what's between the braces:
  int contentEnd = response.indexOf("}", labelStart);
  String content = response.substring(contentStart, contentEnd);
  // now convert it to an int
  int aqi = content.toInt();
  return aqi;
}

void updateLEDs(int airQuality) {
  pixels.clear();
  // Dim the brightness to not consume too much power
  pixels.setBrightness(75);
  
  switch(airQuality) {
    case 1: // Good air quality
      currColor = white;  // White
      break;
    case 2: // Fair air quality
      currColor = green;  // Green
      break;
    case 3: // Moderate air quality
      currColor = yellow;  // Yellow
      break;
    case 4: // Poor air quality
      currColor = orange;  // Orange
      break;  
    case 5: // Very bad air quality
      currColor = red;  // Red
      break;
    default:
      // If the value doesn't make sense, turn off the LEDs
      pixels.clear();
      break;
  }

  // Update LEDs
  for (int i = 0; i < numPixels; i++) {
    pixels.setPixelColor(i, currColor);
    pixels.show();
    delay(100);
  }
}

void heartBeat(uint32_t color) {
    
  // Heart beat - dim
  Serial.println("I'm dimming");
  for (int i = 50; i > 2; i--) {
    pixels.setBrightness(i);
    pixels.fill(color);
    pixels.show();
    delay(50);
  }
  // light up
  Serial.println("I'm lighting up");
  pixels.fill(currColor);
  for (int i = 2; i < 50; i++) {
    pixels.setBrightness(i);
    pixels.fill(color);
    pixels.show(); 
    delay(50);
  }
}

void breatheLungs(int airQuality) {
  switch(airQuality) {
    case 1: // Good air quality
      breathePumps(7000, 3000);
      break;
    case 2: // Fair
      breathePumps(5000, 3000);
      break;
    case 3: // Moderate
      breathePumps(3000, 2000);
      break;
    case 4: // Poor
      breathePumps(2000, 1000);
      break;
    case 5: // Very bad
      breathePumps(1000, 500);
      break;
    default:
      // Debug
      Serial.println("Weird data, don't know what to do");
      break;
  }
}

// These are the pump functions
void inflate(int time) {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(time);
}

void deflate(int time) {
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(time);
}

void turnOffPumps() {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

void breathePumps(int inflateTime, int deflateTime) {
  // Always start with a big inhale
  wdt_reset();
  inflate(8000); // Inflate for 10 seconds

  for (int i = 0; i < 9; i++) {
    wdt_reset();
    deflate(deflateTime);
    inflate(inflateTime);
  }

  // Last deflate
  deflate(deflateTime);
  turnOffPumps();
}
