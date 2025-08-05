/*
 * GameBoard Component Test
 * Tests all hardware components before full game upload
 * 
 * This code verifies:
 * - RFID reader can detect cards
 * - NeoPixel ring displays colors
 * - Buzzer produces audio
 * - All pin connections working
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

#define RST_PIN 9
#define SS_PIN 10
#define BUZZER_PIN 3
#define NEOPIXEL_PIN 6
#define NEOPIXEL_COUNT 12

MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("=== GAMEBOARD COMPONENT TEST ===");
  Serial.println("Testing all hardware components...");
  Serial.println();
  
  // Initialize hardware
  SPI.begin();
  mfrc522.PCD_Init();
  pixels.begin();
  pixels.clear();
  pixels.show();
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Test sequence
  testBuzzer();
  delay(1000);
  
  testNeoPixelRing();
  delay(1000);
  
  testRFIDReader();
  
  Serial.println("=== ALL TESTS COMPLETE ===");
  Serial.println("Scan RFID cards to test detection...");
  Serial.println();
}

void testBuzzer() {
  Serial.print("Testing buzzer... ");
  
  // Short beep pattern
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  delay(100);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  
  Serial.println("✓ PASS (if you heard beeps)");
}

void testNeoPixelRing() {
  Serial.print("Testing NeoPixel ring... ");
  
  // Red sweep
  for(int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.clear();
    pixels.setPixelColor(i, pixels.Color(100, 0, 0)); // Red
    pixels.show();
    delay(100);
  }
  
  // Green sweep  
  for(int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.clear();
    pixels.setPixelColor(i, pixels.Color(0, 100, 0)); // Green
    pixels.show();
    delay(100);
  }
  
  // Blue sweep
  for(int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.clear();
    pixels.setPixelColor(i, pixels.Color(0, 0, 100)); // Blue
    pixels.show();
    delay(100);
  }
  
  // All white flash
  for(int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(50, 50, 50)); // White
  }
  pixels.show();
  delay(500);
  pixels.clear();
  pixels.show();
  
  Serial.println("✓ PASS (if you saw red/green/blue/white)");
}

void testRFIDReader() {
  Serial.print("Testing RFID reader... ");
  
  // Check if RFID module is responding
  byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  if (version == 0x00 || version == 0xFF) {
    Serial.println("✗ FAIL - RFID not responding");
    Serial.println("  Check connections: VCC to 3.3V, all SPI pins");
    return;
  }
  
  Serial.println("✓ PASS - RFID module responding");
  Serial.print("  Firmware version: 0x");
  Serial.println(version, HEX);
}

void loop() {
  // Continuous RFID card detection test
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("🎉 CARD DETECTED!");
    
    // Display card UID
    Serial.print("Card UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    
    // Visual/audio feedback
    cardDetectedFeedback();
    
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1000);
  }
  
  delay(100);
}

void cardDetectedFeedback() {
  // Happy beep
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
  delay(50);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
  
  // Green flash
  for(int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Bright green
  }
  pixels.show();
  delay(300);
  
  // Fade out
  for(int brightness = 255; brightness >= 0; brightness -= 15) {
    for(int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(0, brightness, 0));
    }
    pixels.show();
    delay(30);
  }
}