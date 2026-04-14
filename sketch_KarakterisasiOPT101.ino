#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int laserPin = 8;
int sensorPin = A0;
float voltage;

void setup() {
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, HIGH);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  voltage = sensorValue * (5.0 / 1023.0);

  lcd.setCursor(0, 0);
  lcd.print("Tegangan:");

  lcd.setCursor(0, 1);
  lcd.print(voltage,4);
  lcd.print(" V   ");

  Serial.println(voltage);

  delay(10000);
}