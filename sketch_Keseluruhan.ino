#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin
const int sensorPin = A0;
const int relayPin  = 8;
const int buttonPin = 7;

// Variabel
float tegangan = 0;
float konsentrasi = 0;
bool modeFiltrasi = false; // false = sebelum, true = sesudah

// Fungsi transfer (hasil penelitian)
float hitungKonsentrasi(float V) {
  return (V - 4.3486) / 0.0752;
}

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  digitalWrite(relayPin, LOW); // relay OFF

  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  lcd.setCursor(0,0);
  lcd.print("Sistem Siap...");
  delay(2000);
  lcd.clear();
}

void loop() {

  // ===== Tombol Mode =====
  if (digitalRead(buttonPin) == LOW) {
    modeFiltrasi = !modeFiltrasi;
    delay(300); // debounce
  }

  // ===== Pembacaan Sensor (rata-rata 10x) =====
  int total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(sensorPin);
    delay(5);
  }
  float adc = total / 10.0;

  // Konversi tegangan
  tegangan = adc * (5.0 / 1023.0);

  // Hitung konsentrasi
  konsentrasi = hitungKonsentrasi(tegangan);

  // ===== STATUS =====
  String status;
  if (konsentrasi <= 0.3) {
    status = "AMAN";
  } else {
    status = "TIDAK AMAN";
  }

  // ===== LOGIKA FILTRASI OTOMATIS =====
  if (modeFiltrasi == true) {  
    if (konsentrasi > 0.3) {
      digitalWrite(relayPin, HIGH); // pompa ON
    } else {
      digitalWrite(relayPin, LOW);  // pompa OFF
    }
  } else {
    digitalWrite(relayPin, LOW); // sebelum filtrasi → pompa mati
  }

  // ===== SERIAL =====
  Serial.print("Mode: ");
  Serial.print(modeFiltrasi ? "SESUDAH" : "SEBELUM");
  Serial.print(" | V: ");
  Serial.print(tegangan, 4);
  Serial.print(" | C: ");
  Serial.print(konsentrasi, 3);
  Serial.print(" ppm | ");
  Serial.println(status);

  // ===== LCD =====
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(modeFiltrasi ? "SESudah:" : "SEBelum:");

  lcd.setCursor(0, 1);
  lcd.print(konsentrasi, 3);
  lcd.print("ppm ");
  lcd.print(status);

  delay(1000);
}