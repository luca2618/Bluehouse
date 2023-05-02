// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>


// Replace with your network details
const char* ssid = "Lucas - iPhone";
const char* password = "kal12345";

WiFiClient client;

unsigned long channelID = 2087833;               //your channel
const char* myWriteAPIKey = "NZFMCV2TSYJG7KIC";  // your WRITE API key
const char* server = "api.thingspeak.com";

const int postingInterval = 20 * 1000;  // post data every 20 seconds

#include <ESP_Mail_Client.h>
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
/* The sign in credentials */
#define AUTHOR_EMAIL "YOUR_EMAIL@XXXX.com"
#define AUTHOR_PASSWORD "YOUR_EMAIL_PASS"

/* Recipient's email*/
#define RECIPIENT_EMAIL "lucassylvester02@gmail.com"
SMTPSession smtp;

/* Declare the session config data */
ESP_Mail_Session session;

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

  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
  smtp.debug(1);

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";
}


// runs over and over again
void loop() {
  long Air_humidity = 0;
  long Soil_humidity = 0;
  long CO2 = 0;
  long Light = 0;
  long Water_level = 0;
  long Temperature = 0;
  long Weight = 0;
  long Signal_strength = WiFi.RSSI();

  //Checks
  String alert = "";
  if ((0 >= Air_humidity) || (Air_humidity >= 1))) {
    alert += "Air humidity is " + String(Air_humitidy);
  }


  ThingSpeak.begin(client);
  //api call and server part
  if (client.connect(server, 80)) {

    //Update each field to
    ThingSpeak.setField(1, Air_humidity);
    ThingSpeak.setField(2, Soil_humidity);
    ThingSpeak.setField(3, CO2);
    ThingSpeak.setField(4, Light);
    ThingSpeak.setField(5, Water_level);
    ThingSpeak.setField(6, Temperature);
    ThingSpeak.setField(7, Weight);
    ThingSpeak.setField(8, Signal_strength);

    ThingSpeak.writeFields(channelID, myWriteAPIKey);
  }
  client.stop();

  // wait and then post again
  delay(postingInterval);

  bool email = 1;
  if (email) {
    /* Declare the message class */
    SMTP_Message message;

    /* Set the message headers */
    message.sender.name = "Bluehouse";
    message.sender.email = AUTHOR_EMAIL;
    message.subject = "Bluehouse Alert";
    message.addRecipient("Bluehouse owner", RECIPIENT_EMAIL);

    /*Send HTML message*/
    String htmlMsg = "<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP board</p></div>";
    message.html.content = htmlMsg.c_str();
    message.html.content = htmlMsg.c_str();
    message.text.charSet = "us-ascii";
    message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    /*
  //Send raw text message
  String textMsg = "Hello World! - Sent from ESP board";
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;*/

    /* Set the custom message header */
    //message.addHeader("Message-ID: <abcde.fghij@gmail.com>");

    /* Connect to server with the session config */
    smtp.connect(session);

    /* Start sending Email and close the session */
    if (!MailClient.sendMail(&smtp, &message)) Serial.println("Error sending Email, " + smtp.errorReason());
  }
}
