#include <GyverHub.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <EEPROM.h>
#include <ESPAsyncWebServer.h>
#include <RCSwitch.h>

AsyncWebServer server(80);
TaskHandle_t Task2;
RCSwitch mySwitch = RCSwitch();

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "ALARM_WiFi";
const char* password = "123456789";
//const char* password = "super_strong_password";

const char* PARAM_STRING_1 = "inputString1";
const char* PARAM_STRING_2 = "inputString2";

char data1[200];
char data2[200];
char ssid_STA[200];
char password_STA[200];
int i = 0;

GyverHub hub("MyDevices", "Сигнализация 2", "");  // префикс, имя, иконка
#define SWITCH 22
#define LED_AP 2
#define LED_STA 15
#define S0_2 26
#define S1_2 32
#define S2_2 33
#define S3_2 25
#define EN_2 34
#define SIG_2 19
#define batPin 27
#define chrPin 14
#define SPK 23
#define spkLvlPin 12
const int SER = 16;
const int LATCH = 5;
const int CLK = 18;
int LedOut1 = 0, LedOut2 = 0;
int Pins[16];
int batValue, batValue100 = 0; 
int spkLevel, spkLevel100 = 0;
char level[] = "100 %";
bool chrFlag = 0;
bool AlarmFlag = 0;
bool leds[16];
bool s0, s1, s2, s3;
//WebServer server(80);
bool flag1 = LOW, flag2 = LOW, flag3 = LOW, flag4 = LOW, flag5 = LOW, flag6 = LOW, flag7 = LOW, flag8 = LOW, flag9 = LOW, flag10 = LOW, flag11 = LOW, flag12 = LOW, flag13 = LOW, flag14 = LOW, flag15 = LOW, flag16 = LOW;
bool alarm1 = LOW, alarm2 = LOW, alarm3 = LOW, alarm4 = LOW, alarm5 = LOW, alarm6 = LOW, alarm7 = LOW, alarm8 = LOW, alarm9 = LOW, alarm10 = LOW, alarm11 = LOW, alarm12 = LOW, alarm13 = LOW, alarm14 = LOW, alarm15 = LOW, alarm16 = LOW;
bool LED1status = LOW;
bool LED2status = LOW;
bool SENSOR3status = LOW;
bool sens1, sens2, sens3, sens4, sens5, sens6, sens7, sens8, sens9, sens10, sens11, sens12, sens13, sens14, sens15, sens16;
GHbutton rst;

// HTML web page to handle 2 input fields (inputString++, inputInt-, inputFloat-)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ALARM WiFi input</title>
  <meta name="viewport" content="width=device-width, initial-scale=20">

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

// билдер
void build() {
hub.BeginWidgets();
hub.WidgetSize(70);
hub.Label_(F("Advertisement"), "В случае тревоги нажать кнопку СБРОС", F("."), GH_DEFAULT, 45);
hub.WidgetSize(30);
if (hub.Button_(F("Reset"), &rst, F("Сброс"), GH_ORANGE, 30)) handle_reset();
hub.WidgetSize(20);
hub.SwitchText_(F("sw1"), &sens1, F("Датчик №1 подключен"), F("№1"), GH_DEFAULT);
hub.LED_(F("led1"), 1, "ТРЕВОГА №1");
hub.Space(); 
hub.SwitchText_(F("sw9"), &sens9, F("Датчик №9 подключен"), F("№9"), GH_DEFAULT);
hub.LED_(F("led9"), 1, "ТРЕВОГА №9");
hub.SwitchText_(F("sw2"), &sens2, F("Датчик №2 подключен"), F("№2"), GH_DEFAULT);
hub.LED_(F("led2"), 1, "ТРЕВОГА №2");
hub.Space(); 
hub.SwitchText_(F("sw10"), &sens10, F("Датчик №10 подключен"), F("№10"), GH_DEFAULT);
hub.LED_(F("led10"), 1, "ТРЕВОГА №10");
hub.SwitchText_(F("sw3"), &sens3, F("Датчик №3 подключен"), F("№3"), GH_DEFAULT);
hub.LED_(F("led3"), 1, "ТРЕВОГА №3");
hub.Space(); 
hub.SwitchText_(F("sw11"), &sens11, F("Датчик №11 подключен"), F("№11"), GH_DEFAULT);
hub.LED_(F("led11"), 1, "ТРЕВОГА №11");
hub.SwitchText_(F("sw4"), &sens4, F("Датчик №4 подключен"), F("№4"), GH_DEFAULT);
hub.LED_(F("led4"), 1, "ТРЕВОГА №4");
hub.Space(); 
hub.SwitchText_(F("sw12"), &sens12, F("Датчик №12 подключен"), F("№12"), GH_DEFAULT);
hub.LED_(F("led12"), 1, "ТРЕВОГА №12");
hub.SwitchText_(F("sw5"), &sens5, F("Датчик №5 подключен"), F("№5"), GH_DEFAULT);
hub.LED_(F("led5"), 1, "ТРЕВОГА №5");
hub.Space(); 
hub.SwitchText_(F("sw13"), &sens13, F("Датчик №13 подключен"), F("№13"), GH_DEFAULT);
hub.LED_(F("led13"), 1, "ТРЕВОГА №13");
hub.SwitchText_(F("sw6"), &sens6, F("Датчик №6 подключен"), F("№6"), GH_DEFAULT);
hub.LED_(F("led6"), 1, "ТРЕВОГА №6");
hub.Space(); 
hub.SwitchText_(F("sw14"), &sens14, F("Датчик №14 подключен"), F("№14"), GH_DEFAULT);
hub.LED_(F("led14"), 1, "ТРЕВОГА №14");
hub.SwitchText_(F("sw7"), &sens7, F("Датчик №7 подключен"), F("№7"), GH_DEFAULT);
hub.LED_(F("led7"), 1, "ТРЕВОГА №7");
hub.Space(); 
hub.SwitchText_(F("sw15"), &sens15, F("Датчик №15 подключен"), F("№15"), GH_DEFAULT);
hub.LED_(F("led15"), 1, "ТРЕВОГА №15");
hub.SwitchText_(F("sw8"), &sens8, F("Датчик №8 подключен"), F("№8"), GH_DEFAULT);
hub.LED_(F("led8"), 1, "ТРЕВОГА №8");
hub.Space(); 
hub.SwitchText_(F("sw16"), &sens16, F("Датчик №16 подключен"), F("№16"), GH_DEFAULT);
hub.LED_(F("led16"), 1, "ТРЕВОГА №16");
hub.WidgetSize(70);
hub.Label_(F("Sound"), "Уровень громкости", F("."), GH_DEFAULT, 30);
hub.WidgetSize(30);
hub.Label_(F("Sound_lvl"), "100 %", F("."), GH_DEFAULT, 30);
}

void setup() {
  mySwitch.enableReceive(digitalPinToInterrupt(4));
  pinMode(SPK, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);

  pinMode(SWITCH, INPUT_PULLUP);
  pinMode(LED_AP, OUTPUT);
  pinMode(LED_STA, OUTPUT);

  pinMode(SER, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);

  pinMode(EN_2, OUTPUT);
  pinMode(S0_2, OUTPUT);
  pinMode(S1_2, OUTPUT);
  pinMode(S2_2, OUTPUT); 
  pinMode(S3_2, OUTPUT); 
  pinMode(SIG_2, INPUT_PULLUP);
  Serial.begin(115200);
  EEPROM.begin(512);  // Инициализация EEPROM с размером 512 байт
  delay(100);

  if (digitalRead(SWITCH)){
  digitalWrite(LED_AP, HIGH);
  digitalWrite(LED_STA, LOW);

  //Serial.println("\n[*] Creating AP");
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
      EEPROM.put(0, inputMessage);
      EEPROM.commit();   // Сохранение изменений
    }
    // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(PARAM_STRING_2)) {
      inputMessage = request->getParam(PARAM_STRING_2)->value();
      //writeFile(SPIFFS, "/inputString2.txt", inputMessage.c_str());
      EEPROM.put(255, inputMessage);
      EEPROM.commit();   // Сохранение изменений
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
  else{
  digitalWrite(LED_STA, HIGH);
  digitalWrite(LED_AP, LOW);
  Serial.println("Connecting to ");

  EEPROM.get(0, data1);
  ssid_STA[0] = '"';
  for(i = 0; i<= strlen(data1); i++){
  ssid_STA[i+1] = data1[i];
  }
  ssid_STA[i] = '"';
//Serial.print(ssid_STA);

  EEPROM.get(255, data2);
  password_STA[0] = '"';
  for(i = 0; i<= strlen(data2); i++){
  password_STA[i+1] = data2[i];
  }
  password_STA[i] = '"';
//Serial.print(password_STA);

  WiFi.mode(WIFI_STA);
  WiFi.begin(data1, data2);
  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  //Serial.print(".");
  //Serial.println(WiFi.status());
  //Serial.println(WiFi.begin(ssid, keyIndex, key));
  }
 // Serial.println("WiFi connected..!");
 // Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  //Serial.println("HTTP server started");
  hub.setupMQTT("test.mosquitto.org", 1883);
 // hub.setupMQTT("m6.wqtt.ru", 14426, "u_OD9CWG", "BjkdB1Py");
 // hub.setupMQTT("m4.wqtt.ru", 9022, "u_J2GRP4", "h7A64L6U");
  //hub.setupMQTT("m4.wqtt.ru", 9010, "u_158PW3", "woSIw3vz");
  hub.onBuild(build);     // подключаем билдер
  hub.begin();            // запускаем систему
  }

  xTaskCreatePinnedToCore(
                    Task2code,   /* Функция задачи. */
                    "Task2",     /* Имя задачи. */
                    10000,       /* Размер стека */
                    NULL,        /* Параметры задачи */
                    4,           /* Приоритет */
                    &Task2,      /* Дескриптор задачи для отслеживания */
                    1);          /* Указываем пин для этой задачи */

}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// Replaces placeholder with stored values
String processor(const String& var){
  //Serial.println(var);
  if(var == "inputString1"){
    EEPROM.get(0, data1);
    return data1;
  }
  else if(var == "inputString2"){
    EEPROM.get(255, data2);
    return data2;
  }
  return String();
}

void Shift16Leds()
{
int reg[8] = {128,64,32,16,8,4,2,1};
for (int i = 0; i<=7;i++) 
{
LedOut1 = LedOut1 + (leds[i]*reg[i]);
}
for (int i = 8; i<=15;i++) 
{
LedOut2 = LedOut2 + (leds[i]*reg[i-8]);
}
    digitalWrite(LATCH, LOW);
    shiftOut(SER, CLK, MSBFIRST, LedOut1);
    shiftOut(SER, CLK, MSBFIRST, LedOut2);
    digitalWrite(LATCH, HIGH);
  //  delay(1000);
    LedOut1 = 0;
    LedOut2 = 0;
}
void MUXin()
{
  for (int i=0; i<=15; i++)
  {
  s0 = i%2;
  s1 = i/2%2;
  s2 = i/4%2;
  s3 = i/8%2;
  digitalWrite(EN_2, HIGH);
  digitalWrite(S0_2, s0);
  digitalWrite(S1_2, s1);
  digitalWrite(S2_2, s2);
  digitalWrite(S3_2, s3);
  digitalWrite(EN_2, LOW);
  delay(5);
  Pins[i] = (1-digitalRead(SIG_2))*(i+1);
  leds[i] = 1-digitalRead(SIG_2);
  }
}

void handle_reset() {
  AlarmFlag = 0;
  flag1 = 0;
  alarm1 = 0;
  flag2 = 0;
  alarm2 = 0;
  flag3 = 0;
  alarm3 = 0;
  flag4 = 0;
  alarm4 = 0;
  flag5 = 0;
  alarm5 = 0;
  flag6 = 0;
  alarm6 = 0;
  flag7 = 0;
  alarm7 = 0;
  flag8 = 0;
  alarm8 = 0;
  flag9 = 0;
  alarm9 = 0;
  flag10 = 0;
  alarm10 = 0;
  flag11 = 0;
  alarm11 = 0;
  flag12 = 0;
  alarm12 = 0;
  flag13 = 0;
  alarm13 = 0;
  flag14 = 0;
  alarm14 = 0;
  flag15 = 0;
  alarm15 = 0;
  flag16 = 0;
  alarm16 = 0;
}

void Task2code( void * pvParameters ){

  for(;;){
    Serial.println(batValue);
    Serial.println(spkLevel);

if (alarm1 == 1||alarm2 == 1||alarm3 == 1||alarm4 == 1||alarm5 == 1||alarm6 == 1||alarm7 == 1||alarm8 == 1||alarm9 == 1||alarm10 == 1||alarm11 == 1||alarm12 == 1||alarm13 == 1||alarm14 == 1||alarm15 == 1||alarm16 == 1){
      AlarmFlag = 1;
}
    if (mySwitch.available()) {
      if ((mySwitch.getReceivedValue()== 10437380) || (mySwitch.getReceivedValue()== 5592368))
      {
      AlarmFlag = 0;
      flag1 = 0;
      alarm1 = 0;
      flag2 = 0;
      alarm2 = 0;
      flag3 = 0;
      alarm3 = 0;
      flag4 = 0;
      alarm4 = 0;
      flag5 = 0;
      alarm5 = 0;
      flag6 = 0;
      alarm6 = 0;
      flag7 = 0;
      alarm7 = 0;
      flag8 = 0;
      alarm8 = 0;
      flag9 = 0;
      alarm9 = 0;
      flag10 = 0;
      alarm10 = 0;
      flag11 = 0;
      alarm11 = 0;
      flag12 = 0;
      alarm12 = 0;
      flag13 = 0;
      alarm13 = 0;
      flag14 = 0;
      alarm14 = 0;
      flag15 = 0;
      alarm15 = 0;
      flag16 = 0;
      alarm16 = 0;
      mySwitch.resetAvailable();
      }
    }
    digitalWrite(SPK, AlarmFlag);
    batValue100 = map(batValue, 1450, 2250, 0, 100);
    if(batValue100 > 100)
    {
      batValue100 = 100;
    }
    if(batValue100 < 0)
    {
      batValue100 = 0;
    }
    spkLevel100 = map(spkLevel, 0, 4095, 5, 100);
    Serial.println(batValue100);
    Serial.println(spkLevel100);
    Serial.println(batValue);
    Serial.println(spkLevel);
    Serial.println(chrFlag);
  }
}

void loop() {
  hub.tick();  // обязательно тикаем тут
  static GHtimer tmr(1000);
// for (int count = 0; count < 10000; count++){
  MUXin();
  Shift16Leds();
  if((1-leds[0])==0 && flag1 == 0)
  {flag1 = 1;}
  if((1-leds[1])==0 && flag2 == 0)
  {flag2 = 1;}
  if((1-leds[2])==0 && flag3 == 0)
  {flag3 = 1;}
  if((1-leds[3])==0 && flag4 == 0)
  {flag4 = 1;}
  if((1-leds[4])==0 && flag5 == 0)
  {flag5 = 1;}
  if((1-leds[5])==0 && flag6 == 0)
  {flag6 = 1;}
  if((1-leds[6])==0 && flag7 == 0)
  {flag7 = 1;}
  if((1-leds[7])==0 && flag8 == 0)
  {flag8 = 1;}
  if((1-leds[8])==0 && flag9 == 0)
  {flag9 = 1;}
  if((1-leds[9])==0 && flag10 == 0)
  {flag10 = 1;}
  if((1-leds[10])==0 && flag11 == 0)
  {flag11 = 1;}
  if((1-leds[11])==0 && flag12 == 0)
  {flag12 = 1;}
  if((1-leds[12])==0 && flag13 == 0)
  {flag13 = 1;}
  if((1-leds[13])==0 && flag14 == 0)
  {flag14 = 1;}
  if((1-leds[14])==0 && flag15 == 0)
  {flag15 = 1;}
  if((1-leds[15])==0 && flag16 == 0)
  {flag16 = 1;}
  if (tmr) {
  hub.sendUpdate("sw1", String(flag1));
  hub.sendUpdate("sw2", String(flag2));
  hub.sendUpdate("sw3", String(flag3));
  hub.sendUpdate("sw4", String(flag4));
  hub.sendUpdate("sw5", String(flag5));
  hub.sendUpdate("sw6", String(flag6));
  hub.sendUpdate("sw7", String(flag7));
  hub.sendUpdate("sw8", String(flag8));
  hub.sendUpdate("sw9", String(flag9));
  hub.sendUpdate("sw10", String(flag10));
  hub.sendUpdate("sw11", String(flag11));
  hub.sendUpdate("sw12", String(flag12));
  hub.sendUpdate("sw13", String(flag13));
  hub.sendUpdate("sw14", String(flag14));
  hub.sendUpdate("sw15", String(flag15));
  hub.sendUpdate("sw16", String(flag16));
  hub.sendUpdate("Sound_lvl", String(spkLevel));

    batValue = analogRead(batPin);
    spkLevel = analogRead(spkLvlPin);
    chrFlag = digitalRead(chrPin);

  if(((1-leds[0]) && flag1) || alarm1 == 1)
  {hub.sendUpdate("led1", String(0));
  alarm1 = 1;}
  else
  {hub.sendUpdate("led1", String(1));}

  if(((1-leds[1]) && flag2) || alarm2 == 1)
  {hub.sendUpdate("led2", String(0));
  alarm2 = 1;}
  else
  {hub.sendUpdate("led2", String(1));}

  if(((1-leds[2]) && flag3) || alarm3 == 1)
  {hub.sendUpdate("led3", String(0));
  alarm3 = 1;}
  else
  {hub.sendUpdate("led3", String(1));}

  if(((1-leds[3]) && flag4) || alarm4 == 1)
  {hub.sendUpdate("led4", String(0));
  alarm4 = 1;}
  else
  {hub.sendUpdate("led4", String(1));}

  if(((1-leds[4]) && flag5) || alarm5 == 1)
  {hub.sendUpdate("led5", String(0));
  alarm5 = 1;}
  else
  {hub.sendUpdate("led5", String(1));}

  if(((1-leds[5]) && flag6) || alarm6 == 1)
  {hub.sendUpdate("led6", String(0));
  alarm6 = 1;}
  else
  {hub.sendUpdate("led6", String(1));}

  if(((1-leds[6]) && flag7) || alarm7 == 1)
  {hub.sendUpdate("led7", String(0));
  alarm7 = 1;}
  else
  {hub.sendUpdate("led7", String(1));}

  if(((1-leds[7]) && flag8) || alarm8 == 1)
  {hub.sendUpdate("led8", String(0));
  alarm8 = 1;}
  else
  {hub.sendUpdate("led8", String(1));}

  if(((1-leds[8]) && flag9) || alarm9 == 1)
  {hub.sendUpdate("led9", String(0));
  alarm9 = 1;}
  else
  {hub.sendUpdate("led9", String(1));}

  if(((1-leds[9]) && flag10) || alarm10 == 1)
  {hub.sendUpdate("led10", String(0));
  alarm10 = 1;}
  else
  {hub.sendUpdate("led10", String(1));}

  if(((1-leds[10]) && flag11) || alarm11 == 1)
  {hub.sendUpdate("led11", String(0));
  alarm11 = 1;}
  else
  {hub.sendUpdate("led11", String(1));}

  if(((1-leds[11]) && flag12) || alarm12 == 1)
  {hub.sendUpdate("led12", String(0));
  alarm12 = 1;}
  else
  {hub.sendUpdate("led12", String(1));}

  if(((1-leds[12]) && flag13) || alarm13 == 1)
  {hub.sendUpdate("led13", String(0));
  alarm13 = 1;}
  else
  {hub.sendUpdate("led13", String(1));}

  if(((1-leds[13]) && flag14) || alarm14 == 1)
  {hub.sendUpdate("led14", String(0));
  alarm14 = 1;}
  else
  {hub.sendUpdate("led14", String(1));}

  if(((1-leds[14]) && flag15) || alarm15 == 1)
  {hub.sendUpdate("led15", String(0));
  alarm15 = 1;}
  else
  {hub.sendUpdate("led15", String(1));}

  if(((1-leds[15]) && flag16) || alarm16 == 1)
  {hub.sendUpdate("led16", String(0));
  alarm16 = 1;}
  else
  {hub.sendUpdate("led16", String(1));}
  }
}
