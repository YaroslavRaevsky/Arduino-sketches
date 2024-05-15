#include <GyverHub.h>
#include <RCSwitch.h>

GyverHub hub("MyDevices", "ESP8266", "");  // префикс, имя, иконка
RCSwitch mySwitch = RCSwitch();
#define batPin 27
#define chrPin 14
#define SPK 23
#define spkLvlPin 12
#define SWITCH 22

int batValue, batValue100 = 0; 
int spkLevel, spkLevel100 = 0;
bool chrFlag = 0;
bool AlarmFlag = 0;

//TaskHandle_t Task1;
TaskHandle_t Task2;

// билдер
void build() {
}

void setup() {
  hub.onBuild(build);     // подключаем билдер
  hub.begin();            // запускаем систему
  Serial.begin(115200); 
  pinMode(SPK, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);
  mySwitch.enableReceive(digitalPinToInterrupt(4));

  //создаем задачу, которая будет выполняться на ядре 0 с максимальным приоритетом (1)
 /* xTaskCreatePinnedToCore(
                    Task1code,   // Функция задачи. 
                    "Task1",     // Ее имя. 
                    10000,       // Размер стека функции 
                    NULL,        // Параметры 
                    1,           // Приоритет 
                    &Task1,      // Дескриптор задачи для отслеживания 
                    0);          // Указываем пин для данного ядра                   
  delay(500); */

  //Создаем задачу, которая будет выполняться на ядре 1 с наивысшим приоритетом (1)
  xTaskCreatePinnedToCore(
                    Task2code,   /* Функция задачи. */
                    "Task2",     /* Имя задачи. */
                    10000,       /* Размер стека */
                    NULL,        /* Параметры задачи */
                    1,           /* Приоритет */
                    &Task2,      /* Дескриптор задачи для отслеживания */
                    1);          /* Указываем пин для этой задачи */
    delay(500); 
}

/*void Task1code( void * pvParameters ){
 // Serial.print("Task1 running on core ");
 // Serial.println(xPortGetCoreID());

/*for(;;)/*{
    delay(500);
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
    Serial.println(chrFlag);
if ((mySwitch.getReceivedValue()== 10437380) || (mySwitch.getReceivedValue()== 5592368))
{
digitalWrite(SPK, chrFlag);
mySwitch.resetAvailable();
}  
  } 
}*/

void Task2code( void * pvParameters ){

  for(;;){
    batValue = analogRead(batPin);
    spkLevel = analogRead(spkLvlPin);
    chrFlag = digitalRead(chrPin);

if (digitalRead(SWITCH)){
      AlarmFlag = 1;
}
    if (mySwitch.available()) {
      if ((mySwitch.getReceivedValue()== 10437380) || (mySwitch.getReceivedValue()== 5592368))
      {
      AlarmFlag = 0;
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
    Serial.println(chrFlag);
  }
}

void loop() {
    hub.tick();  // обязательно тикаем тут
  static GHtimer tmr(1000);
    delay(5000);
}