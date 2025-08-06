/*
 * GAMEBOARD - With Real Ranking Logic
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

enum GameState {
  WAITING_FOR_CHALLENGE,
  SCANNING_TOKENS,
  GAME_COMPLETE
};

GameState currentState = WAITING_FOR_CHALLENGE;
int currentScore = 0;
int tokensScanned = 0;
bool gameActive = false;
int currentChallengeType = -1;

// Track scanned tokens
int scannedTokens[10];  // Store indices of scanned tokens
int lastScannedValue = 0;  // Value of last scanned token

// Challenge UIDs and names
char challengeUIDs[4][25] = {
  "1D C2 30 76 0D 10 80",  // GDP
  "1D E1 4C 76 0D 10 80",  // Population  
  "1D D8 3F 76 0D 10 80",  // Area
  "1D D1 3C 76 0D 10 80"   // GDP per Capita
};

char challengeNames[4][20] = {
  "GDP Challenge",
  "Population Challenge", 
  "Area Challenge",
  "GDP per Capita"
};

// Token data with country stats
char tokenUIDs[10][25] = {
  "1D 5B 00 76 0D 10 80",  // USA
  "1D 0E 0E 76 0D 10 80",  // China
  "1D EC 10 76 0D 10 80",  // Japan
  "1D 05 1E 76 0D 10 80",  // Germany
  "1D 0D 21 76 0D 10 80",  // India
  "1D 43 2E 76 0D 10 80",  // UK
  "1D 23 31 76 0D 10 80",  // France
  "1D 30 3D 76 0D 10 80",  // Italy
  "1D 39 40 76 0D 10 80",  // Brazil
  "1D 42 4D 76 0D 10 80"   // Canada
};

char tokenNames[10][20] = {
  "United States", "China", "Japan", "Germany", "India",
  "United Kingdom", "France", "Italy", "Brazil", "Canada"
};

int tokenGDP[10] = {21427, 14342, 5081, 3846, 2875, 2829, 2716, 2001, 1869, 1736};
int tokenPop[10] = {331, 1439, 125, 84, 1380, 67, 67, 60, 215, 38};
int tokenArea[10] = {9834, 9597, 378, 357, 3287, 244, 549, 301, 8516, 9985};

unsigned long lastScanTime = 0;
char lastUID[25] = "";

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println(F("=== GAMEBOARD WITH REAL RANKING LOGIC ==="));
  
  SPI.begin();
  mfrc522.PCD_Init();
  
  pixels.begin();
  pixels.clear();
  pixels.show();
  pinMode(BUZZER_PIN, OUTPUT);
  
  playWelcomeSound();
  
  Serial.println(F("🎮 Scan a challenge card to begin!"));
  Serial.println(F(""));
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  
  if (!mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
  
  char uid[25];
  getUIDString(uid);
  
  if (millis() - lastScanTime < 1500 || strcmp(uid, lastUID) == 0) {
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(50);
    return;
  }
  
  Serial.print(F("📡 Scanned: "));
  Serial.println(uid);
  
  handleCardScan(uid);
  
  strcpy(lastUID, uid);
  lastScanTime = millis();
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(100);
}

void getUIDString(char* buffer) {
  buffer[0] = '\0';
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (i > 0) strcat(buffer, " ");
    char hex[4];
    sprintf(hex, "%02X", mfrc522.uid.uidByte[i]);
    strcat(buffer, hex);
  }
}

void handleCardScan(char* uid) {
  if (currentState == WAITING_FOR_CHALLENGE) {
    int challengeIndex = findChallenge(uid);
    if (challengeIndex >= 0) {
      startChallenge(challengeIndex);
    } else {
      playErrorBuzz();
      flashRed();
      Serial.println(F("❌ Please scan a challenge card!"));
    }
  } else if (currentState == SCANNING_TOKENS) {
    int challengeIndex = findChallenge(uid);
    int tokenIndex = findToken(uid);
    
    if (challengeIndex >= 0) {
      endGame();
    } else if (tokenIndex >= 0) {
      // Check if token was already scanned
      bool alreadyScanned = false;
      for (int i = 0; i < tokensScanned; i++) {
        if (scannedTokens[i] == tokenIndex) {
          alreadyScanned = true;
          break;
        }
      }
      
      if (alreadyScanned) {
        playErrorBuzz();
        flashRed();
        Serial.println(F("❌ Token already scanned!"));
      } else {
        scanToken(tokenIndex);
      }
    } else {
      playErrorBuzz();
      flashRed();
      Serial.println(F("❌ Unknown card!"));
    }
  }
}

int findChallenge(char* uid) {
  for (int i = 0; i < 4; i++) {
    if (strcmp(challengeUIDs[i], uid) == 0) {
      return i;
    }
  }
  return -1;
}

int findToken(char* uid) {
  for (int i = 0; i < 10; i++) {
    if (strcmp(tokenUIDs[i], uid) == 0) {
      return i;
    }
  }
  return -1;
}

long getTokenValue(int index) {
  switch(currentChallengeType) {
    case 0: return tokenGDP[index];  // GDP
    case 1: return tokenPop[index];   // Population
    case 2: return tokenArea[index];  // Area
    case 3: // GDP per Capita (in dollars)
      // Cast to long first to avoid overflow
      long gdp = (long)tokenGDP[index];
      long pop = (long)tokenPop[index];
      return (gdp * 1000L) / pop;
    default: return 0;
  }
}

void startChallenge(int index) {
  currentState = SCANNING_TOKENS;
  currentScore = 0;
  tokensScanned = 0;
  gameActive = true;
  currentChallengeType = index;
  lastScannedValue = 999999;  // Start with very high value for "highest to lowest"
  
  // Clear scanned tokens array
  for (int i = 0; i < 10; i++) {
    scannedTokens[i] = -1;
  }
  
  playSuccessSound();
  challengeAnimation();
  
  Serial.println(F("🎯=================================🎯"));
  Serial.print(F("🎮 CHALLENGE: "));
  Serial.println(challengeNames[index]);
  Serial.print(F("📊 Ranking: "));
  
  switch(index) {
    case 0: Serial.println(F("Countries by GDP (Highest to Lowest)")); break;
    case 1: Serial.println(F("Countries by Population (Highest to Lowest)")); break;
    case 2: Serial.println(F("Countries by Area (Highest to Lowest)")); break;
    case 3: Serial.println(F("Countries by GDP per Capita (Highest to Lowest)")); break;
  }
  
  Serial.println(F("🎯=================================🎯"));
  Serial.println(F("🎲 Start scanning country tokens..."));
  Serial.println(F(""));
  
  // Show correct ranking for reference (only in debug mode)
  showCorrectRanking();
}

void showCorrectRanking() {
  Serial.println(F("📋 CORRECT RANKING (for reference):"));
  
  // Create temporary array for sorting
  int indices[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  
  // Sort indices based on current challenge type
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      long value_i = getTokenValue(indices[i]);
      long value_j = getTokenValue(indices[j]);
      
      if (value_j > value_i) {  // Highest to lowest
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
      }
    }
  }
  
  // Display sorted ranking
  for (int i = 0; i < 10; i++) {
    Serial.print(i + 1);
    Serial.print(F(". "));
    Serial.print(tokenNames[indices[i]]);
    Serial.print(F(" ("));
    
    long value = getTokenValue(indices[i]);
    switch(currentChallengeType) {
      case 0: Serial.print(F("$")); Serial.print(value); Serial.print(F("B")); break;
      case 1: Serial.print(value); Serial.print(F("M")); break;
      case 2: Serial.print(value); Serial.print(F("k km²")); break;
      case 3: Serial.print(F("$")); Serial.print(value); break;
    }
    
    Serial.println(F(")"));
  }
  Serial.println(F(""));
}

void scanToken(int index) {
  scannedTokens[tokensScanned] = index;
  tokensScanned++;
  
  // Show detailed country information
  Serial.println(F("📊=================================📊"));
  Serial.print(F("🏷️  Country: "));
  Serial.println(tokenNames[index]);
  Serial.print(F("💰 GDP: $"));
  Serial.print(tokenGDP[index]);
  Serial.println(F(" billion"));
  Serial.print(F("👥 Population: "));
  Serial.print(tokenPop[index]);
  Serial.println(F(" million"));
  Serial.print(F("🗺️  Area: "));
  Serial.print(tokenArea[index]);
  Serial.println(F(" thousand km²"));
  
  // Calculate and show value for current challenge
  long currentValue = getTokenValue(index);
  Serial.print(F("📈 Value for this challenge: "));
  
  switch(currentChallengeType) {
    case 0: // GDP
      Serial.print(F("$"));
      Serial.print(currentValue);
      Serial.println(F(" billion"));
      break;
    case 1: // Population
      Serial.print(currentValue);
      Serial.println(F(" million"));
      break;
    case 2: // Area
      Serial.print(currentValue);
      Serial.println(F(" thousand km²"));
      break;
    case 3: // GDP per Capita
      Serial.print(F("$"));
      Serial.println(currentValue);
      break;
  }
  
  Serial.println(F("📊=================================📊"));
  
  scanningAnimation();
  playScanningSound();
  delay(800);
  
  // Check if the ranking is correct
  bool isCorrect = false;
  
  if (tokensScanned == 1) {
    // First token just needs to be valid
    isCorrect = true;
  } else {
    // Check if current value is less than or equal to previous (for highest to lowest)
    isCorrect = (currentValue <= lastScannedValue);
  }
  
  // Always update lastScannedValue if correct, for next comparison
  if (isCorrect) {
    lastScannedValue = currentValue;
  }
  
  if (isCorrect) {
    currentScore += (tokensScanned * 10);
    
    playCorrectBuzz();
    flashGreen();
    
    Serial.print(F("✅ CORRECT! Position #"));
    Serial.print(tokensScanned);
    Serial.print(F(" - "));
    Serial.print(tokenNames[index]);
    Serial.print(F(" | Score: "));
    Serial.println(currentScore);
    
    // Show why it's correct
    if (tokensScanned > 1) {
      Serial.print(F("   (Current: "));
      Serial.print(currentValue);
      Serial.print(F(" ≤ Previous: "));
      Serial.print(lastScannedValue);
      Serial.println(F(" ✓)"));
    }
    
    Serial.println(F("🎲 Scan next token or challenge card to bank!"));
    Serial.println(F(""));
    
  } else {
    gameActive = false;
    currentScore = 0;
    
    playWrongBuzz();
    flashRed();
    
    Serial.print(F("❌ WRONG! "));
    Serial.print(tokenNames[index]);
    Serial.println(F(" is out of order!"));
    
    // Show why it's wrong
    Serial.print(F("   ("));
    Serial.print(currentValue);
    Serial.print(F(" > "));
    Serial.print(lastScannedValue);
    Serial.println(F(" - should be lower!)"));
    
    Serial.println(F("💥 GAME OVER!"));
    Serial.println(F(""));
    
    currentState = WAITING_FOR_CHALLENGE;
    Serial.println(F("🎮 Scan challenge card to play again!"));
  }
}

void endGame() {
  if (gameActive && tokensScanned > 0) {
    playVictoryBuzz();
    bankingAnimation();
    
    Serial.println(F("🏆 POINTS BANKED!"));
    Serial.print(F("💰 Final Score: "));
    Serial.println(currentScore);
    Serial.print(F("🎯 Tokens Scanned: "));
    Serial.println(tokensScanned);
  }
  
  currentState = WAITING_FOR_CHALLENGE;
  currentChallengeType = -1;
  Serial.println(F("🎮 Scan new challenge card for next round!"));
  Serial.println(F(""));
}

// Sound functions
void buzz(int duration) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

void playWelcomeSound() {
  buzz(200); delay(100);
  buzz(200); delay(100);
  buzz(300);
}

void playSuccessSound() {
  buzz(150); delay(50);
  buzz(200);
}

void playErrorBuzz() {
  buzz(600);
}

void playCorrectBuzz() {
  buzz(100); delay(50);
  buzz(100); delay(50);
  buzz(200);
}

void playWrongBuzz() {
  buzz(800);
}

void playVictoryBuzz() {
  buzz(100); delay(50);
  buzz(100); delay(50);
  buzz(100); delay(50);
  buzz(200);
}

void playScanningSound() {
  for (int i = 0; i < 5; i++) {
    buzz(40);
    delay(60);
  }
}

// Light functions
void challengeAnimation() {
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 100));
  }
  pixels.show();
  delay(500);
  pixels.clear();
  pixels.show();
}

void scanningAnimation() {
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.clear();
    pixels.setPixelColor(i, pixels.Color(100, 100, 0));
    pixels.show();
    delay(60);
  }
}

void flashGreen() {
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 255, 0));
  }
  pixels.show();
  delay(400);
  pixels.clear();
  pixels.show();
}

void flashRed() {
  for (int flash = 0; flash < 3; flash++) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    }
    pixels.show();
    delay(150);
    pixels.clear();
    pixels.show();
    delay(150);
  }
}

void bankingAnimation() {
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 215, 0));
  }
  pixels.show();
  delay(1000);
  pixels.clear();
  pixels.show();
}