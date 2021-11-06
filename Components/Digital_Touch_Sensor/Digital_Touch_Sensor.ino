#define TouchSensor 9
boolean currentState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(TouchSensor, INPUT);
}

void loop() {
  currentState = digitalRead(TouchSensor);
  if (currentState == HIGH){
  Serial.println("Tıklandı.");
  delay(200);
  }
}
