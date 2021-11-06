const int prob = A0;
int olcum_sonucu; 

void setup() {
   Serial.begin(9600);
}

void loop() {
  olcum_sonucu = analogRead(prob);
  if (olcum_sonucu<700){
  Serial.print("Okunan deger = ");
  Serial.print(olcum_sonucu);
  Serial.println("   (ISLAK) ");
  delay(2000);
  }
  else {
  Serial.print("Okunan deger = ");
  Serial.print(olcum_sonucu);
  Serial.println("   (KURU) ");
  delay(2000);
  }
}
