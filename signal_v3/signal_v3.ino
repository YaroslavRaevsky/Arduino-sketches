#include <WiFi.h>
#include <WebServer.h>
/* Установите здесь свои SSID и пароль */
const char* ssid = "RT-GPON-27D0";  
const char* password = "D3bh3hdjig";  
WebServer server(80);
uint8_t LED1pin = 4;
bool LED1status = LOW;
uint8_t LED2pin = 5;
bool LED2status = LOW;
bool SENSOR3status = LOW;
bool flag1 = LOW, flag2 = LOW, flag3 = LOW;
bool alarm1 = LOW, alarm2 = LOW, alarm3 = LOW;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  pinMode(15, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);
  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  Serial.println(WiFi.status());
//  Serial.println(WiFi.begin(ssid, keyIndex, key));
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/sensor2off", handle_sensor2off);
  server.on("/sensor2check", handle_sensor2check);
  server.on("/sensor2alarm", handle_sensor2alarm);  
  server.on("/sensor3off", handle_sensor3off);
  server.on("/sensor3check", handle_sensor3check);
  server.on("/sensor3alarm", handle_sensor3alarm);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
 // LED1status = 1;
 // LED2status = 0;
}
void loop() {
  server.handleClient();

  if(digitalRead(15)==0 && flag3 == 0)
  {flag3 = 1;}
  if(digitalRead(2)==0 && flag2 == 0)
  {flag2 = 1;}
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}
}
void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,SENSOR3status)); 
}
void handle_led1on() {
  LED1status = HIGH;
  flag3 = 0;
  alarm3 = 0;
  flag2 = 0;
  alarm2 = 0;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status,SENSOR3status)); 
}
void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status,SENSOR3status)); 
}

void handle_sensor2off() {
  SENSOR3status = HIGH;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,1)); 
}
void handle_sensor2check() {
  SENSOR3status = LOW;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,2)); 
}
void handle_sensor2alarm() {
  SENSOR3status = LOW;
  server.send(200, "text/html", SendHTML(LED1status,LED2status,3)); 
}
void handle_sensor3off() {
  SENSOR3status = HIGH;
  server.send(200, "text/html", SendHTML(LED1status,LED2status,1)); 
}
void handle_sensor3check() {
  SENSOR3status = LOW;
  server.send(200, "text/html", SendHTML(LED1status,LED2status,2)); 
}
void handle_sensor3alarm() {
  SENSOR3status = LOW;
 // flag1 = 0;
 // alarm1 = 0;  
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status,3)); 
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
String SendHTML(uint8_t led1stat, uint8_t led2stat, uint8_t sensor3stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<meta http-equiv=\"Refresh\" content=\"1\" />\n";
  ptr +="<title>Сигнализация</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 10px;} h1 {color: #444444;margin: 10px auto 10px;}\n"; //h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 200px;background-color: #7CEC00;border: none;color: white;padding: 5px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 0px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #9D2EFE;}\n";
  ptr +=".button-check:on {background-color: #9D2EFE;}\n";
  ptr +=".button-check {background-color: #7CEC00;}\n";
  ptr +=".button-check:active {background-color: #7CEC00;}\n";
  ptr +=".button-alarm {background-color: #ff0000;}\n";
  ptr +=".button-alarm:active {background-color: #ff0000;}\n"; 
  ptr +=".button-off {background-color: #909090;}\n";
  ptr +=".button-off:active {background-color: #909090;}\n";
  ptr +="p {font-size: 20px;color: #444444;margin-bottom: 1px; margin-top: 3px}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Сигнализация</h1>\n";
  //  ptr +="<h3>Режим станции (STA)</h3>\n";
   if(led1stat)
  {ptr +="<p>Подключите датчики и нажмите ПЕРЕЗАПУСК</p><a class=\"button button-off\" href=\"/led1off\">ПЕРЕЗАПУСК</a>\n";}
  else
  {ptr +="<p>Нажмите СБРОС в случае тревоги</p><a class=\"button button-on\" href=\"/led1on\">СБРОС</a>\n";}

  if((digitalRead(2) && flag2) || alarm2 == 1)
  {ptr +="<p>Датчик №2</p><a class=\"button button-alarm\" href=\"/sensor2alarm\">ТРЕВОГА!</a>\n";
  alarm2 = 1;}
  else if (digitalRead(2)==0)
  {ptr +="<p>Датчик №2</p><a class=\"button button-check\" href=\"/sensor2check\">ОХРАНА</a>\n";}
  else
  {ptr +="<p>Датчик №2</p><a class=\"button button-off\" href=\"/sensor2off\">ВЫКЛЮЧЕН</a>\n";}
 
  if((digitalRead(15) && flag3) || alarm3 == 1)
  {ptr +="<p>Датчик №3</p><a class=\"button button-alarm\" href=\"/sensor3alarm\">ТРЕВОГА!</a>\n";
  alarm3 = 1;}
  else if (digitalRead(15)==0)
  {ptr +="<p>Датчик №3</p><a class=\"button button-check\" href=\"/sensor3check\">ОХРАНА</a>\n";}
  else
  {ptr +="<p>Датчик №3</p><a class=\"button button-off\" href=\"/sensor3off\">ВЫКЛЮЧЕН</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
