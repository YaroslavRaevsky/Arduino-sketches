int pushButton = 2;
void setup() {
 Serial.begin(9600); // put your setup code here, to run once:
 Serial.println("Hello, world!");
   pinMode(pushButton, INPUT);
  pinMode(5, OUTPUT);
}
int currentValue, prevValue;
void loop() {
  currentValue = digitalRead(pushButton);
  if (currentValue != prevValue) {
    // Что-то изменилось, здесь возможна зона неопределенности
    // Делаем задержку
    delay(10);
    // А вот теперь спокойно считываем значение, считая, что нестабильность исчезла
    currentValue = digitalRead(pushButton);
  }
  prevValue = currentValue;
  digitalWrite(5, currentValue);
  Serial.println(currentValue);
}
