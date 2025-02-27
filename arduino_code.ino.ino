#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10  
#define RST_PIN 9  

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Cart");
  lcd.setCursor(0, 1);
  lcd.print("Ready to scan...");
}

void loop() {
  
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    if (Serial.available() != 0) {
  String response = Serial.readStringUntil('\n');  

  // Split response into product name & total price
  int separatorIndex = response.indexOf(",");
  if (separatorIndex > 0) {
    String productName = response.substring(0, separatorIndex);
    String totalPrice = response.substring(separatorIndex + 1);

    // Display product name and total price on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Item: " + productName);
    lcd.setCursor(0, 1);
    lcd.print("Total: " + totalPrice + " DA");

    delay(2000);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Cart");
  lcd.setCursor(0, 1);
  lcd.print("Ready to scan...");
  }
    return;
  }

  // Read UID and format as a string
  String tagUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");  
    tagUID += String(mfrc522.uid.uidByte[i], HEX);  
  }

  Serial.println(tagUID); // Send UID to Python
  delay(500);

  // Wait for response from Python (product name + total price)
  while (Serial.available() == 0) {}  
  String response = Serial.readStringUntil('\n');  

  // Split response into product name & total price
  int separatorIndex = response.indexOf(",");
  if (separatorIndex > 0) {
    String productName = response.substring(0, separatorIndex);
    String totalPrice = response.substring(separatorIndex + 1);

    // Display product name and total price on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Item: " + productName);
    lcd.setCursor(0, 1);
    lcd.print("Total: " + totalPrice + " DA");

    delay(2000);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Cart");
  lcd.setCursor(0, 1);
  lcd.print("Ready to scan...");
}
