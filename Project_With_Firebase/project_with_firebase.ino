#include <ESP8266WiFi.h>      // ESP8266WiFi kitapligi dahil edildi
#include <FirebaseArduino.h>  // Firebase kitapligi dahil edildi
#include <Servo.h>            // Servo kitapligi dahil edildi
#include "NewPing.h"          // NewPing kitapligi dahil edildi

#define TRIGGER_PIN D2        // Ultrasonik sensorunun Trigger ucu NodeMCU' nun D2 pinine baglandi  
#define ECHO_PIN D1           // Ultrasonik sensorunun Echo ucu NodeMCU' nun D1 pinine baglandi  
#define MAX_DISTANCE 400      // Ping atmak istedigimiz maksimum mesafe (santimetre olarak)
 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Pinler ve maksimum mesafe kullanilarak NewPing kurulumu
float distance;               // "float" tipinde, "distance" adinda degisken tanimlandi

Servo servo;                  // "Servo" tipinde, "servo" adinda bir nesne olusturuldu

const int prob = A0;          // "int" tipinde, "prob" adinda sabit bir degisken tanimlandi ve A0 pinini tanımlandi
                              // Nem sensorunun A0 ucunu NodeMCU' nun A0 pinine baglandi
int olcum_sonucu;             // prob' tan gelen degerlerin kaydedilmesi icin "int" tipinde, "olcum_sonucu" adinda 
                              // bir degisken tanimlandi

#define FIREBASE_HOST "***************************************"   // Baglanilacak olan firebase host adresi
#define FIREBASE_AUTH "****************************************"  // Baglanilacak olan firebase key
#define WIFI_SSID "*****"          // Baglanilacak olan wifi aginin adi 
#define WIFI_PASSWORD "*********"  // Baglanilacak olan wifi aginin sifresi
                          
void setup() {                     // Genel ayarlar buradan yapilir 
  servo.attach(D0);                // Servo motor NodeMCU' nun D0 pinine baglandiginin belirtilip baslasilmasi 
  Serial.begin(9600);              // Seri haberlesmenin baslatilmasi

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);         // SSID si yukarıda belirtilmis olan wifi a baglan
  Serial.print("connecting");                   // Ekrana "connecting" yazdirildi
  while (WiFi.status() != WL_CONNECTED) {       // Baglanti kuruldu mu?
    Serial.print(".");                          // Ekrana "." yazdirildi
    delay(500);                                 // 0.5 saniye bekle
  }
  Serial.println();                             // Bir satir bos birakilmasi icin kullanildi
  Serial.print("connected: ");                  // Ekrana "connected: " yazdirildi
  Serial.println(WiFi.localIP());               // Wifi agindan alinan IP adresini yazdirildi

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Firebase veritabani ile baglanti gerceklestirildi
}

int atiksayisi = 1;       // Toplam atik sayisinin kaydedilmesi icin "int" tipinde, "atiksayisi" adinda bir degisken tanimlandi
int kuruatiksayisi = 1;   // Kuru atik sayisinin kaydedilmesi icin "int" tipinde, "kuruatiksayisi" adinda bir degisken tanimlandi
int islakatiksayisi = 1;  // Islak atik sayisinin kaydedilmesi icin "int" tipinde, "islakatiksayisi" adinda bir degisken tanimlandi

void loop() {             // Arduino'nun yapmasi gereken islemler buraya yazilir 
  distance = sonar.ping_cm();           // Ping gönderip mesafeyi cm cinsinden alinmasi
  Serial.print("Mesafe = ");            // Ekrana "Mesafe = " yazdirildi
  if (distance >= 400 || distance <= 2){// Eger ultrasonik sensorden alinan veri 2 cm ile 400 cm arasinda bir deger ise  
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
    
      Firebase.setFloat("Toplam Atık Sayısı", atiksayisi);    // "Toplam Atık Sayısı" etiketine "atiksayisi" degiskeninde bulunan
                                                              // veri yazdirildi
      if (Firebase.failed()) {                                // Eger isleme hatasi meydana gelirse bu komut satiri calistirilsin
          Serial.print("Yazdirma basarisiz :");               // Ekrana "Yazdirma basarisiz :" yazdirildi
          Serial.println(Firebase.error());                   // Ekrana Firebase hata kodu yazdirildi
          return;                                             // Donus saglandi
      }

      Firebase.setFloat("Islak Atık Sayısı", islakatiksayisi);// "Islak Atık Sayısı" etiketine "islakatiksayisi" degiskeninde 
                                                              // bulunan veri yazdirildi
      if (Firebase.failed()) {                                // Eger isleme hatasi meydana gelirse bu komut satiri calistirilsin
          Serial.print("Yazdirma basarisiz :");               // Ekrana "Yazdirma basarisiz :" yazdirildi
          Serial.println(Firebase.error());                   // Ekrana Firebase hata kodu yazdirildi
          return;                                             // Donus saglandi
      }
      atiksayisi++;                                           // "atiksayisi" degiskeninin degeri 1 arttirildi
      islakatiksayisi++;                                      // "islakatiksayisi" degiskeninin degeri 1 arttirildi
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
      
      Firebase.setFloat("Toplam Atık Sayısı", atiksayisi);    // "Toplam Atık Sayısı" etiketine "atiksayisi" degiskeninde bulunan
                                                              // veri yazdirildi
      if (Firebase.failed()) {                                // Eger isleme hatasi meydana gelirse bu komut satiri calistirilsin
          Serial.print("Yazdirma basarisiz :");               // Ekrana "Yazdirma basarisiz :" yazdirildi
          Serial.println(Firebase.error());                   // Ekrana Firebase hata kodu yazdirildi
          return;                                             // Donus saglandi
      }

      Firebase.setFloat("Kuru Atık Sayısı", kuruatiksayisi);  // "Kuru Atık Sayısı" etiketine "kuruatiksayisi" degiskeninde 
                                                              // bulunan veri yazdirildi
      if (Firebase.failed()) {                                // Eger isleme hatasi meydana gelirse bu komut satiri calistirilsin
          Serial.print("Yazdirma basarisiz :");               // Ekrana "Yazdirma basarisiz :" yazdirildi
          Serial.println(Firebase.error());                   // Ekrana Firebase hata kodu yazdirildi
          return;                                             // Donus saglandi
      }
      atiksayisi++;                                           // "atiksayisi" degiskeninin degeri 1 arttirildi
      kuruatiksayisi++;                                       // "kuruatiksayisi" degiskeninin degeri 1 arttirildi
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
