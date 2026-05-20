# GasGuard
Gas Detection and Early Warning System designed with Arduino
KOMPAKT GAZ ALGILAMA VE ERKEN UYARI SİSTEMİ

PROJENİN AMACI VE GEREKÇESİ 
Günümüzde ev kazalarının önemli bir kısmını, açık unutulan ocaklar veya tesisat 
arızalarından kaynaklanan gaz kaçakları oluşturmaktadır. Özellikle mutfak gibi kapalı 
alanlarda biriken gaz, belirli bir seviyeye ulaştığında patlama veya zehirlenme riski 
doğurmaktadır. Bu projenin temel amacı, bu tür tehlikeleri henüz başlangıç aşamasındayken 
tespit edip, kullanıcıyı hem görsel hem de işitsel olarak uyaran, duvara monte edilebilir 
(kompakt) bir güvenlik modülü tasarlamaktır. 
Tasarlanan "GasGuard" sistemi, ortamdaki gaz seviyesini sürekli analiz eder. Gaz kaçağı 
başladığında, tehlike henüz ölümcül seviyeye gelmeden önce kullanıcıyı kademeli olarak 
uyarır. Bu sistem, özellikle yaşlı bireylerin yaşadığı evlerde veya dalgınlık sonucu 
oluşabilecek kazaların önüne geçilmesinde hayati bir rol oynamayı hedeflemektedir. 
KULLANILAN MALZEMELER VE DONANIM 
Proje kapsamında aşağıdaki elektronik bileşenler kullanılmıştır: 
 Arduino Uno : Sistemin beyni olarak tüm sensör verilerini işler ve çıktıları kontrol 
eder. 
 MQ-2 Gaz Sensörü: Ortamdaki yanıcı gazları ve dumanı algılayarak analog sinyal 
üretir. 
 16x2 LCD Ekran (I2C Modüllü): Gaz seviyesini ve sistem durumunu 
(Güvenli/Uyari/Tehlike) yazılı olarak gösterir. I2C modülü sayesinde kablo 
karmaşası en aza indirilmiştir. 
 LED Göstergeler: 
o Yeşil LED: Güvenli durumu temsil eder. 
o Sarı LED: Uyarı durumunu temsil eder. 
o Kırmızı LED: Acil tehlike durumunu temsil eder. 
 Buzzer (Piezo): Tehlike anında sesli alarm verir. 
 Güç Kaynağı: 9V Pil ve Pil Başlığı (Sistemin taşınabilir olmasını sağlar). 
 Aç/Kapa Anahtarı (Rocker Switch): Sistemin enerjisini kontrol etmek için güç hattına 
entegre edilmiştir. 
 Yardımcı Elemanlar: Breadboard, Jumper Kablolar, Dirençler (220Ω ve 4.7kΩ). 

<img width="553" height="328" alt="image" src="https://github.com/user-attachments/assets/9c35ca8e-5755-43b0-bbe5-3114ca3c98e4" />

PROJENİN ÇALIŞMA PRENSİBİ 
Sistem, "Giriş - İşlem - Çıkış" döngüsü ile çalışır: 
1. Veri Okuma (Giriş): MQ-2 gaz sensörü, ortamdaki gaz yoğunluğunu ölçer ve 
Arduino'nun A0 pinine analog bir değer (0-1023 arası) gönderir. 
2. Karar Verme (İşlem): Arduino, yazılan kod sayesinde bu değeri sürekli analiz eder. 
Belirlenen eşik değerlerine (Güvenli Sınır ve Tehlike Sınırı) göre hangi aksiyonun 
alınacağına karar verir. 
3. Uyarı Sistemi (Çıkış): 
o Gaz seviyesi güvenli sınırın altındaysa Yeşil LED yanar, ekran "GÜVENLİ" 
yazar. 
o Seviye artmaya başlarsa Sarı LED yanıp söner ve Buzzer yavaş tempoda ikaz 
verir. 
o Seviye tehlikeli boyuta ulaşırsa Kırmızı LED ve Buzzer çok hızlı şekilde 
tetiklenir, LCD ekranda "ORAYI TERK EDİN" uyarısı belirir. 
SİSTEM SENARYOLARI VE ALGORİTMA 
Yazılımda millis() fonksiyonu kullanılarak sistemin aynı anda hem ölçüm yapması hem de 
LED/Buzzer yakıp söndürmesi (Multitasking) sağlanmıştır. Sistem 3 aşamalı çalışır: 
 Aşama 1: Güvenli Mod  
o Görsel: Yeşil LED Sabit Yanar. 
o LCD: "DURUM: GUVENLI" ve anlık gaz değeri gösterilir. 
o Ses: Yok. 
 Aşama 2: Uyarı Modu  
o Görsel: Sarı LED 0.3 saniye aralıklarla yanıp söner. 
o LCD: "UYARI! YUKSELIYOR" mesajı verilir. 
o Ses: Buzzer, LED ile eş zamanlı "bip-bip" yapar. 
 Aşama 3: Alarm Modu  
o Görsel: Kırmızı LED 0.1 saniye aralıklarla (çok hızlı) yanıp söner. 
o LCD: "!!! TEHLIKE !!! ORAYI TERK EDIN!" uyarısı çıkar. Gaz değeri takip 
edilmeye devam edilir. 
o Ses: Buzzer yüksek frekansta ve hızda alarm çalar.
<img width="507" height="277" alt="image" src="https://github.com/user-attachments/assets/1b75b709-0b2d-4faf-a7d7-19bff3614872" />
3D Kutu Tasarımı ve Fiziksel Montaj 
Projenin sadece teorik bir devre olarak kalmaması ve gerçek hayatta güvenle kullanılabilir, 
estetik bir "ürün" haline gelmesi hedeflenmiştir. Bu amaçla, elektronik bileşenleri içine alacak 
kompakt bir muhafaza (kutu) tasarımı yapılmıştır. 
Tasarım, 3 boyutlu (3D) yazıcı teknolojisi kullanılarak PLA/ABS malzeme ile üretilmiştir. 
Devre elemanları, sensörün dışarıdan hava alabileceği ve ekranın net görünebileceği şekilde 
bu özel kutu içerisine monte edilmiştir. Bu aşama, sistemin taşınabilir ve duvara asılabilir 
nihai formuna kavuşmasını sağlamıştır.
<img width="566" height="856" alt="image" src="https://github.com/user-attachments/assets/69b5b9b1-fc77-4d84-9c1c-8d579fa5aa10" />

SONUÇ 
Bu çalışmada, mikroişlemci tabanlı bir gaz güvenlik sistemi başarıyla prototiplenmiştir. 
Yapılan testlerde sistemin şu başarımları sağladığı görülmüştür: 
1. Hassasiyet: Sensör, gaz seviyesindeki küçük değişimleri (analog değer olarak) anlık 
olarak LCD ekrana yansıtmıştır. 
2. Kademeli Uyarı: Tek bir alarm yerine, "Uyarı" ve "Tehlike" olarak iki farklı 
kademenin olması, kullanıcıya paniğe kapılmadan önlem alma şansı tanımaktadır. 
3. Kullanıcı Arayüzü: I2C LCD ekran kullanımı sayesinde, karmaşık kablo bağlantıları 
azaltılmış ve kullanıcıya net bilgi aktarımı sağlanmıştır.
