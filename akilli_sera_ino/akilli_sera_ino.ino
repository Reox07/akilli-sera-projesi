// AYKAN KARA - AKILLI SERA PROJESİ
// Bu proje, bir sera'daki ışık, toprak nemi, ortam sıcaklığı ve nemi ölçerek
// belirli değerlere göre röle ile çalışan cihazları kontrol etmektedir.

#include <DHT.h> // DHT11 sıcaklık ve nem sensörünün kütüphanesi

// Sensör pin tanımları
int LDR = A0;         // LDR sensörü analog A0 pinine bağlandı. Ortam ışığını ölçer.
int TOPRAK = A1;      // Toprak nem sensörü analog A1 pinine bağlandı. Topraktaki nemi ölçer.
int DHTPIN = 2;       // DHT11 sensörünün veri pini dijital D2 pinine bağlandı.

#define DHTTYPE DHT11 // Kullanılan sensör tipi DHT11 olarak tanımlanıyor
DHT dht(DHTPIN, DHTTYPE); // DHT sensörü için nesne oluşturuluyor

// Röle çıkış pin tanımları
int FAN = 3;          // Fan rölesi D3 pinine bağlandı. Ortam sıcaklığına göre çalışır. (şimdilik aktif değil)
int SU = 4;           // Su pompası rölesi D4 pinine bağlandı. Toprak nemine göre çalışır.
int LED = 5;          // LED rölesi D5 pinine bağlandı. Ortam ışık şiddetine göre çalışır.

// Cihazların açık/kapalı durumlarını takip eden değişkenler
bool ledDurumu = false;     // LED başta kapalı
bool suDurumu = false;      // Su pompası başta kapalı
bool fanDurumu = false;     // Fan başta kapalı (ileride kullanılacak)

void setup() {
  // Sensör pinlerini giriş olarak tanımladık
  pinMode(LDR, INPUT);
  pinMode(TOPRAK, INPUT);
  dht.begin(); // DHT11 sensörü başlatılıyor

  // Röle pinlerini çıkış olarak tanımladık
  pinMode(FAN, OUTPUT);
  pinMode(SU, OUTPUT);
  pinMode(LED, OUTPUT);

  // Başlangıçta tüm röleler kapalı
  digitalWrite(FAN, LOW);
  digitalWrite(SU, LOW);
  digitalWrite(LED, LOW);

  // Seri iletişimi başlat
  Serial.begin(9600);
  Serial.println("SERA ÇALIŞMAYA BAŞLADI.");
}

void loop() {
  // Sensörlerden gelen değerleri oku
  int ISIK = analogRead(LDR);                // Ortam ışığı (0–1023)
  int NEM = analogRead(TOPRAK);              // Toprak nem değeri (0–1023)
  float SICAKLIK = dht.readTemperature();    // Ortam sıcaklığı (°C)
  float HAVA_NEMI = dht.readHumidity();      // Ortam nem oranı (%)

  // Sensör değerlerini tek satırda göster
  Serial.print("IŞIK = ");
  Serial.print(ISIK);
  Serial.print(" | TOPRAK NEMİ = ");
  Serial.print(NEM);
  Serial.print(" | SICAKLIK = ");
  Serial.print(SICAKLIK);
  Serial.print(" °C | HAVA NEMİ = ");
  Serial.print(HAVA_NEMI);
  Serial.println(" %");

  // Ortam ışığına göre LED kontrolü
  // Eğer ışık seviyesi 700 veya üzerindeyse ortam karanlıktır ve LED açılır
  // 500 veya altındaysa ortam aydınlıktır ve LED kapanır
  // Durum sadece değişince röle tetiklenir
  if (ISIK >= 700 && !ledDurumu) {
    digitalWrite(LED, HIGH);     // LED açılır
    ledDurumu = true;
    Serial.println("LED AÇILDI");
  }
  else if (ISIK <= 500 && ledDurumu) {
    digitalWrite(LED, LOW);      // LED kapatılır
    ledDurumu = false;
    Serial.println("LED KAPANDI");
  }

  // Toprak nemine göre su pompası kontrolü
  // Eğer toprak çok kuruysa (nem > 700), pompa çalıştırılır
  // Toprak yeterince nemliyse (nem < 500), pompa durdurulur
  if (NEM >= 700 && !suDurumu) {
    digitalWrite(SU, HIGH);      // Pompa açılır
    suDurumu = true;
    Serial.println("SU POMPASI AÇILDI");
  }
  else if (NEM <= 500 && suDurumu) {
    digitalWrite(SU, LOW);       // Pompa kapatılır
    suDurumu = false;
    Serial.println("SU POMPASI KAPANDI");
  }

    // Ortam sıcaklığına göre fan kontrolü
   // Eğer sıcaklık 30°C veya üzerindeyse fan açılır
  // Sıcaklık 28°C veya altına düştüğünde fan kapatılır
 // Böylece ortam aşırı ısınmadan korunur ve enerji tasarrufu sağlanır
// Fan durumu sadece değiştiğinde güncellenir ve seri monitöre yazdırılır
if (SICAKLIK >= 30 && !fanDurumu){
  digitalWrite(FAN,HIGH); // Fan açılır 
  fanDurumu = true;
  Serial.println(" FAN AÇILDI");
}
else if (SICAKLIK <= 28 && fanDurumu){
  digitalWrite(FAN,LOW); // Fan kapatılır
  fanDurumu = false;
  Serial.println(" FAN KAPANDI");
}
delay(1500); // Bekleme olarak 1 saniye girildi 
}