/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-input-data-html-form/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "ALARM_WiFi";
const char* password = "123456789";

const char* PARAM_STRING_1 = "inputString1";
const char* PARAM_STRING_2 = "inputString2";

// HTML web page to handle 3 input fields (inputString, inputInt, inputFloat)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=10">

  <form action="/get" target="hidden-form">
    WiFi name <input type="text" name="inputString1">
    <input type="submit" value="Submit WiFi name" onclick="document.location.reload(true)">
    <p class="margin-bottom-20"></p>
    saved WiFi name: %inputString1% 
    <p class="margin-bottom-20"></p>
    <p class="margin-bottom-20"></p>
    <p class="margin-bottom-20"></p>
  </form><br>
  <form action="/get" target="hidden-form">
    Password <input type="text" name="inputString2">
    <input type="submit" value="Submit Password" onclick="document.location.reload(true)">
    <p class="margin-bottom-20"></p>
    saved Password: %inputString2%
  </form><br>

  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

// Replaces placeholder with stored values
String processor(const String& var){
  //Serial.println(var);
  if(var == "inputString1"){
    return readFile(SPIFFS, "/inputString1.txt");
  }
  else if(var == "inputString2"){
    return readFile(SPIFFS, "/inputString2.txt");
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  // Initialize SPIFFS
    if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }

    Serial.println("\n[*] Creating AP");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    Serial.print("[+] AP Created with IP Gateway ");
    Serial.println(WiFi.softAPIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
    if (request->hasParam(PARAM_STRING_1)) {
      inputMessage = request->getParam(PARAM_STRING_1)->value();
      writeFile(SPIFFS, "/inputString1.txt", inputMessage.c_str());
    }
    // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(PARAM_STRING_2)) {
      inputMessage = request->getParam(PARAM_STRING_2)->value();
      writeFile(SPIFFS, "/inputString2.txt", inputMessage.c_str());
    }

    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // To access your stored values on inputString, inputInt, inputFloat
  String yourInputString1 = readFile(SPIFFS, "/inputString1.txt");
  Serial.print("*** Your inputString1: ");
  Serial.println(yourInputString1);
  
  String yourInputString2 = readFile(SPIFFS, "/inputString2.txt");
  Serial.print("*** Your inputString2: ");
  Serial.println(yourInputString2);
  delay(5000);
}