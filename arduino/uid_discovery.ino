/*
 * GameBoard UID Discovery Tool
 * Scan NFC stickers and RFID cards to discover their UIDs
 * Use this to program your tokens and challenge cards
 * 
 * Instructions:
 * 1. Upload this code to Arduino
 * 2. Open Serial Monitor (9600 baud)
 * 3. Scan each NFC sticker/card individually
 * 4. Record the displayed UID for each token
 * 5. Update the main game code with actual UIDs
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

int cardCount = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  // Initialize hardware
  SPI.begin();
  mfrc522.PCD_Init();
  pixels.begin();
  pixels.clear();
  pixels.show();
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Welcome sequence
  Serial.println("=== GAMEBOARD UID DISCOVERY TOOL ===");
  Serial.println();
  Serial.println("This tool helps you discover the UIDs of your NFC stickers");
  Serial.println("and RFID cards for programming into the game code.");
  Serial.println();
  Serial.println("Instructions:");
  Serial.println("1. Scan each NFC sticker/card individually");
  Serial.println("2. Record the UID shown for each token");
  Serial.println("3. Update gameboard_prototype.ino with actual UIDs");
  Serial.println();
  Serial.println("Ready! Scan your first card/sticker...");
  Serial.println("==============================================");
  Serial.println();
  
  // Ready indication
  readyAnimation();
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    cardCount++;
    
    Serial.print("CARD #");
    Serial.print(cardCount);
    Serial.println(" DETECTED:");
    
    // Display UID in format used by game code
    Serial.print("UID: \"");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (i > 0) Serial.print(" ");
      if (mfrc522.uid.uidByte[i] < 0x10) Serial.print("0");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println("\"");
    
    // Display raw hex for debugging
    Serial.print("Raw: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print("0x");
      if (mfrc522.uid.uidByte[i] < 0x10) Serial.print("0");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      if (i < mfrc522.uid.size - 1) Serial.print(", ");
    }
    Serial.println();
    
    // Card type info
    Serial.print("Card type: ");
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));
    
    Serial.println("----------------------------------------------");
    Serial.println();
    
    // Visual/audio feedback
    scanConfirmation();
    
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1500);
  }
  
  delay(100);
}

void readyAnimation() {
  // Blue pulse to indicate ready
  for(int cycle = 0; cycle < 2; cycle++) {
    for(int brightness = 20; brightness <= 100; brightness += 10) {
      for(int i = 0; i < NEOPIXEL_COUNT; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, brightness));
      }
      pixels.show();
      delay(50);
    }
    for(int brightness = 100; brightness >= 20; brightness -= 10) {
      for(int i = 0; i < NEOPIXEL_COUNT; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, brightness));
      }
      pixels.show();
      delay(50);
    }
  }
  pixels.clear();
  pixels.show();
}

void scanConfirmation() {
  // Happy beep
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150);
  digitalWrite(BUZZER_PIN, LOW);
  
  // Yellow flash (discovery/scanning color)
  for(int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 0)); // Yellow
  }
  pixels.show();
  delay(400);
  
  // Fade to green (success)
  for(int step = 0; step <= 20; step++) {
    int yellow = 255 - (step * 12);
    int green = 255;
    for(int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(yellow, green, 0));
    }
    pixels.show();
    delay(30);
  }
  
  // Final green
  for(int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Green
  }
  pixels.show();
  delay(300);
  
  pixels.clear();
  pixels.show();
}

/*
=== PROGRAMMING LOG TEMPLATE ===

Copy this template and fill in the UIDs as you scan:

// Token Database - Update with your actual UIDs
Token tokens[] = {
  {"[UID1]", "United States", "USA", 21427, 331, 9834},
  {"[UID2]", "China", "CHN", 14342, 1439, 9597},
  {"[UID3]", "Japan", "JPN", 5081, 125, 378},
  {"[UID4]", "Germany", "GER", 3846, 84, 357},
  {"[UID5]", "India", "IND", 2875, 1380, 3287},
  // Add more countries as needed...
};

// Challenge Database - Update with your actual UIDs  
Challenge challenges[] = {
  {"[CHALLENGE_UID1]", "GDP Challenge", CHALLENGE_GDP, true},
  {"[CHALLENGE_UID2]", "Population Challenge", CHALLENGE_POPULATION, true},
  {"[CHALLENGE_UID3]", "Area Challenge", CHALLENGE_AREA, true},
  {"[CHALLENGE_UID4]", "GDP per Capita", CHALLENGE_GDP_PER_CAPITA, true}
};

Replace [UID1], [UID2], etc. with the actual UIDs discovered by this tool.
*/