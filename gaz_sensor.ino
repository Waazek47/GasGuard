#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// --- AYARLAR ---
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// --- BUZZER AYARI ---
// Buzzer hala ters çalışıyorsa (susturunca ötüyorsa)
// aşağıdaki HIGH ve LOW yerlerini değiştirmek yeterli.
#define BUZZER_ACIK  HIGH
#define BUZZER_KAPALI LOW

// Pinler
const int sensorPin = A0;
const int yesilLed = 2;
const int sariLed = 3;
const int kirmiziLed = 4;
const int buzzerPin = 5;

// Zamanlayıcılar
unsigned long oncekiZaman = 0;
bool ledDurumu = false; 

void setup() {
  pinMode(yesilLed, OUTPUT);
  pinMode(sariLed, OUTPUT);
  pinMode(kirmiziLed, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Başlangıçta buzzer sussun
  digitalWrite(buzzerPin, BUZZER_KAPALI);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Kalibrasyon...");
  delay(2000); // Sensör kendine gelsin diye azıcık bekleme
  lcd.clear();
}

void loop() {
  int gazDegeri = analogRead(sensorPin);
  unsigned long suankiZaman = millis();
  
  // --- DURUM 1: GÜVENLİ (< 160) ---
  if (gazDegeri < 160) {
    digitalWrite(yesilLed, HIGH);  // Yeşil Yanar
    digitalWrite(sariLed, LOW);
    digitalWrite(kirmiziLed, LOW);
    digitalWrite(buzzerPin, BUZZER_KAPALI); // Ses Yok
    
    // LCD Mesajı
    lcd.setCursor(0, 0);
    lcd.print("DURUM: GUVENLI  "); 
    lcd.setCursor(0, 1);
    lcd.print("Gaz Level: ");
    lcd.print(gazDegeri);
    lcd.print("   "); // Sayı küçülürse eski rakamlar kalmasın diye boşluk
    
    ledDurumu = false;
  }
  
  // --- DURUM 2: UYARI (160 - 200 Arası) ---
  else if (gazDegeri >= 160 && gazDegeri <= 200) {
    digitalWrite(yesilLed, LOW);
    digitalWrite(kirmiziLed, LOW);
    
    // Sarı ışık ve Buzzer YAVAŞ yanıp sönecek (500ms)
    if (suankiZaman - oncekiZaman >= 200) {
      oncekiZaman = suankiZaman;
      ledDurumu = !ledDurumu; 
      
      digitalWrite(sariLed, ledDurumu ? HIGH : LOW);
      digitalWrite(buzzerPin, ledDurumu ? BUZZER_ACIK : BUZZER_KAPALI);
    }
    
    // LCD Mesajı
    lcd.setCursor(0, 0);
    lcd.print("UYARI! YUKSELIYOR"); // Üstte uyarı
    lcd.setCursor(0, 1);
    lcd.print("Gaz Level: ");       // Altta değeri görmeye devam et
    lcd.print(gazDegeri);
    lcd.print("   ");
  }
  
  // --- DURUM 3: TEHLİKE (> 200) ---
  else if (gazDegeri > 200) {
    digitalWrite(yesilLed, LOW);
    digitalWrite(sariLed, LOW);
    
    // Kırmızı ışık ve Buzzer HIZLI yanıp sönecek (100ms)
    if (suankiZaman - oncekiZaman >= 100) {
      oncekiZaman = suankiZaman;
      ledDurumu = !ledDurumu; 
      
      digitalWrite(kirmiziLed, ledDurumu ? HIGH : LOW);
      digitalWrite(buzzerPin, ledDurumu ? BUZZER_ACIK : BUZZER_KAPALI);
    }
    
    // LCD Mesajı - Burada sayı yok, kaçın! :)
    lcd.setCursor(0, 0);
    lcd.print("!!! TEHLIKE !!! ");
    lcd.setCursor(0, 1);
    lcd.print("ORAYI TERK EDIN!");
  }
  
  delay(50); 
}
