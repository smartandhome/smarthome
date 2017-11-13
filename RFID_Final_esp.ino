/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <FS.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
char* payload[]={}; 

String filename = "/S/";

#define SS_PIN 4
#define RST_PIN 5
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 

const char* ssid     = "tejas";
const char* password = "wificonnected243";

const char* host = "192.168.1.15";


void setup() {
  SPI.begin(); // Init SPI bus
  SPIFFS.begin();
  rfid.PCD_Init(); // Init MFRC522
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  } 
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
   if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
    
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  HTTPClient http;
  USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://192.168.1.15/mydata.txt"); //HTTP
        
        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                StaticJsonBuffer<200> jsonBuffer;
                // Step 2: Deserialize the JSON string
                //
                JsonObject& root = jsonBuffer.parseObject(payload);

                if (!root.success())
                  {
                  Serial.println("parseObject() failed");
                  return;
                 }
              // Step 3: Retrieve the values

              const char* cmd    = root["cmd"];
              const char* data   = root["data"];
              USE_SERIAL.println(cmd);
              USE_SERIAL.println(data);
              if (strcmp(cmd, "add")  == 0) {
                
               // filename = "/P/";
                filename = data;
                File f = SPIFFS.open(filename, "a+");
                // Check if we created the file
                if (f) {
                  f.close(); // We found it, close the file
                  USE_SERIAL.println("file created");
                  }
                  else{
                     USE_SERIAL.println("could not create file");
                    }
              }
              else if (strcmp(cmd, "remove")  == 0) {
               
                filename = data;
                SPIFFS.remove(filename);
                USE_SERIAL.print(filename);
                USE_SERIAL.print(" removed");
              } 
              else
              {
                Serial.println("Invalid Data input");
                }
              
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
        
   Serial.println(F("\n Scanning PICC's UID:"));
  // We now create a URI for the request
   String url = "/checkin.php";
   url += "?ID=";
   String tag;
   Serial.print("Requesting URL: ");
  // This will send the request to the server
   Serial.println(url);
   
  for ( uint8_t i = 0; i < 4; i++) {  //
     tag += rfid.uid.uidByte[i];     
  }
  url += tag;
  Serial.println(tag);
  int isKnown = 0;  // First assume we don't know until we got a match
  filename = tag;
  File f = SPIFFS.open(filename, "r");
  // Check if we could find it above function returns true if the file is exist
  if (f) {
    f.close(); // We found it so close the file
    isKnown = 1; // Label it as known
    // We may also want to do something else if we know the UID
    // Open a door lock, turn a servo, etc
    Serial.println("valid Entry, Access granted");
  }
  else{
    Serial.println("Invalid Entry");
    }  
  //client.println(" ");
 
 
 
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  while(client.available()){
    String line = client.readStringUntil('\r');// Read all the lines of the reply from server and print them to Serial
    Serial.print(line);
  }
  
  // Read all the lines of the reply from server and print them to Serial

  Serial.println();
  Serial.println("closing connection");
}

