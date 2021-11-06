#include <Servo.h>

Servo servo;

void setup() {
  servo.attach(8);
}

void loop() {
servo.write(0);
delay(1000);
servo.write(45);
delay(1000);
servo.write(90);
delay(1000);
servo.write(135);
delay(1000);
servo.write(180);
delay(1000);
}
