#include <GyverHub.h>
#include <WiFi.h>
//#define GH_NO_MQTT
GyverHub hub("MyDevices", "Сигнализация", "");  // префикс, имя, иконка
GHbutton btn17, btn18;
bool sens14;

// билдер
void build() {
Serial.begin(115200);
//for (int i = 0; i < 5; i++) {
//  hub.Button();
//}

  hub.BeginWidgets();
  hub.WidgetSize(100);
 /* hub.Button(0, F("Датчик 1"));  hub.Button(0, F("Датчик 2"));  hub.Button(0, F("Датчик 3"));  hub.Button(0, F("Датчик 4"));
  hub.WidgetSize(25);
  hub.Button(0, F("Sensor 5"));  hub.Button(0, F("Sensor 6"));  hub.Button(0, F("Sensor 7"));  hub.Button(0, F("Sensor 8"));
  hub.WidgetSize(25);
  hub.Button(0, F("Sensor 9"));  hub.Button(0, F("Sensor 10"));  hub.Button(0, F("Sensor 11"));  hub.Button(0, F("Sensor 12"));
  hub.WidgetSize(25);
  hub.Button(0, F("Sensor 13"));  hub.Button(0, F("Датчик 14"));  hub.Button(0, F("Датчик 15"));  hub.Button(0, F("Sensor 16"));
*/

if (hub.Button(&btn17, F("Перезапуск"), GH_RED)) Serial.println(btn17);
hub.WidgetSize(50);
hub.LED_(F("Sensor_1"), 0, "Датчик №1");
hub.Button_(F("sens2"), &btn18, F("Датчик №2"), GH_RED);

hub.Label_(F("lbl"));
hub.Switch();
hub.SwitchIcon();
hub.SwitchText();

hub.LED_(F("led"));
hub.SwitchText_(F("sw"));
hub.SwitchText_(F("sw2"), 0, F("swh"), F("start"), 0);
hub.SwitchText_(F("sw3"), 0, F("Датчик"), F("№3"), GH_ORANGE);
hub.SwitchText_(F("sw14"), &sens14, F("Датчик"), F("№14"), GH_ORANGE);
//hub.SwitchText_(FSTR name, bool* var = 0, FSTR label = 0, FSTR text = 0, uint32_t color = GH_DEFAULT);

//SwitchIcon(0, 0, 0, GH_DEFAULT);
//SwitchText(0, qwe, FSTR text = 0, GH_DEFAULT);
//GHbutton btn18;
//hub.Button_(But, &btn18, F("Датчик 18"), GH_RED);    // подключить переменную и установить цвет
//if (hub.Button(But)) Serial.println(btn18);

//for (int i = 0; i < 5; i++) {
//  if (hub.Button()) Serial.println(i);
//}
  // в теле билдера код будет выполняться в любом случае

}

void setup() {
     Serial.begin(115200);

        WiFi.mode(WIFI_STA);
WiFi.begin("RT-GPON-27D0", "D3bh3hdjig");
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
}
    // подключение к WiFi...
    // настройка MQTT
    hub.setupMQTT("test.mosquitto.org", 1883);
   // hub.setupMQTT("m8.wqtt.ru", 13448, "****", "****");

    hub.onBuild(build);     // подключаем билдер
    hub.begin();            // запускаем систему

}

void loop() {
    hub.tick();  // обязательно тикаем тут
  // вывод значения millis() в лейбл lbl раз в секунду
  static GHtimer tmr(1000);
//  if (tmr) hub.sendUpdate("sens2", String(millis()));
//  if (tmr) hub.sendUpdate("lbl", String(millis())); Serial.println(sens14);
if (tmr) {
  hub.sendUpdate("sw14", String(1));
  delay(2000);
  hub.sendUpdate("sw14", String(0));
  delay(2000);
  }
}

