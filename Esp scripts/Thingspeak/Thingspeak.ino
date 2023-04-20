// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>


// Replace with your network details
const char* ssid = "Lucas - iPhone";
const char* password = "kal12345";

WiFiClient client;

unsigned long channelID = 2087833; //your channel
const char * myWriteAPIKey = "NZFMCV2TSYJG7KIC"; // your WRITE API key
const char* server = "api.thingspeak.com";

const int postingInterval = 20 * 1000; // post data every 20 seconds

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
}

// runs over and over again
void loop() {
  ThingSpeak.begin(client);
  //api call and server part
  if (client.connect(server, 80)) {
    long Air_humidity = 0;
    long Soil_humidity = 0;
    long CO2 = 0;
    long Light = 0;
    long Water_level = 0;
    long Temperature = 0;
    long Weight = 0;
    long Signal_strength = WiFi.RSSI();

    ThingSpeak.setField(1,Air_humidity);
    ThingSpeak.setField(2,Soil_humidity);
    ThingSpeak.setField(3,CO2);
    ThingSpeak.setField(4,Light);
    ThingSpeak.setField(5,Water_level);
    ThingSpeak.setField(6,Temperature);
    ThingSpeak.setField(7,Weight);
    ThingSpeak.setField(8,rssi);

    ThingSpeak.writeFields(channelID, myWriteAPIKey);
  }
    client.stop();

  // wait and then post again
  delay(postingInterval);
}
