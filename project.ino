#include <Servo.h>        // Servo kitapligi dahil edildi
#include "NewPing.h"      // NewPing kitapligi dahil edildi

#define TRIGGER_PIN 12    // Ultrasonik sensorunun Trigger ucunu Arduino Uno' nun 12. pinine baglandi  
#define ECHO_PIN 11       // Ultrasonik sensorunun Echo ucunu Arduino Uno' nun 11. pinine baglandi  
#define MAX_DISTANCE 400  // Ping atmak istedigimiz maksimum mesafe (santimetre olarak)
 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Pinler ve maksimum mesafe kullanilarak NewPing kurulumu
float distance;           // "float" tipinde, "distance" adinda degisken tanimlandi

Servo servo;              // "Servo" tipinde, "servo" adinda bir nesne olusturuldu

const int prob = A0;      // "int" tipinde, "prob" adinda sabit bir degisken tanimlandi ve A0 pinini tanımlandi
                          // Nem sensorunun A0 ucunu Arduino Uno' nun A0. pinine baglandi
int olcum_sonucu;         // prob' tan gelen degerlerin kaydedilmesi icin "int" tipinde, "olcum_sonucu" adinda 
                          // bir degisken tanimlandi
void setup() {            // Genel ayarlar buradan yapilir 
  servo.attach(8);        // Servo motor Arduino Uno' nun 8. pinine baglandiginin belirtilip baslasilmasi 
  Serial.begin(9600);     // Seri haberlesmenin baslatilmasi
}

void loop() {             // Arduino'nun yapmasi gereken islemler buraya yazilir 
  distance = sonar.ping_cm();           // Ping gönderip mesafeyi cm cinsinden alinmasi
  Serial.print("Mesafe = ");            // Ekrana "Mesafe = " yazdirildi
  if (distance >= 400 || distance <= 2){// Ultrasonik sensorden alinan veri 2 cm ile 400 cm arasinda bir deger ise  
                                        // bu komut satiri calistirilsin 
       Serial.println("Out of range");  // Ekrana "Out of range" yazdirildi
  }
  else{                                 // Onceki belirlenen durumdan farkli bir durum disinda bu komut satiri calistirilsin
      Serial.print(distance);           // Ultrasonik sensorden alinan veri "distance" degiskeni kullanilarak ekrana yazdirildi 
      Serial.println(" cm");            // Ekrana " cm" yazdirildi
  }
  delay(500);                           // 0.5 saniye bekle
  olcum_sonucu = analogRead(prob);      // Nem sensorune ait veriler "prob" pininden analog olarak okundu
                                        // ve "olcum_sonucu" isimli degiskene kaydedildi
  if (olcum_sonucu<850 ){               // Eger "olcum_sonucu" degiskeninde bulunan veri, 850' den kucuk ise bu 
                                        // komut satiri calistirilsin 
      Serial.print("Okunan deger = ");  // Ekrana "Okunan deger = " yazdirildi
      Serial.print(olcum_sonucu);       // Nem sensorunden okunan veri "olcum_sonucu" degiskeni kullanilarak ekrana yazdirildi
      Serial.println("   (ISLAK) ");    // Ekrana "   (ISLAK) " yazdirildi
      Serial.println(" ");              // Bir satir bos birakilmasi icin kullanildi
      delay(1000);                      // 1 saniye bekle
      servo.write(0);                   // Servo motor, 0 derece konumuna gonderildi 
      delay(1000);                      // 1 saniye bekle
      servo.write(35);                  // Servo motor, 35 derece konumuna gonderildi
   }
   else if(olcum_sonucu>850 && distance <=9 && distance >= 1  ) {    // Eger "olcum_sonucu" degiskeninde bulunan veri, 850' den 
                                        // buyuk ve ultrasonik sensorden alinan veri 1 cm ile 9 cm arasinda bir deger ise  
                                        // bu komut satiri calistirilsin 
      Serial.print("Okunan deger = ");  // Ekrana "Okunan deger = " yazdirildi
      Serial.print(olcum_sonucu);       // Nem sensorunden okunan veri "olcum_sonucu" degiskeni kullanilarak ekrana yazdirildi
      Serial.println("   (KURU) ");     // Ekrana "   (KURU) " yazdirildi
      Serial.println(" ");              // Bir satir bos birakilmasi icin kullanildi
      delay(1000);                      // 1 saniye bekle
      servo.write(70);                  // Servo motor, 70 derece konumuna gonderildi
      delay(1000);                      // 1 saniye bekle
      servo.write(35);                  // Servo motor, 35 derece konumuna gonderildi
   }
   else{                               // Onceki belirlenen durumlardan farkli bir durum disinda bu komut satiri calistirilsin 
      Serial.print("Okunan deger = ");  // Ekrana "Okunan deger = " yazdirildi
      Serial.print(olcum_sonucu);       // Nem sensorunden okunan veri "olcum_sonucu" degiskeni kullanilarak ekrana yazdirildi
      Serial.println("   (SABIT) ");    // Ekrana "   (SABIT) " yazdirildi
      Serial.println(" ");              // Bir satir bos birakilmasi icin kullanildi
      delay(2000);                      // 2 saniye bekle
      servo.write(35);                  // Servo motor, 35 derece konumuna gonderildi
   }
}
