/*
 * Know-It-All DIY Prototype - Press Your Luck RFID Game
 * Arduino-based RFID ranking game with immediate scan feedback
 * 
 * Hardware Requirements:
 * - Arduino Uno R3 Official
 * - MFRC522 RFID Reader (13.56MHz)
 * - WS2812B NeoPixel Ring (12 LEDs) - Pin 6
 * - Active Buzzer Module - Pin 3
 * - 9V Battery Holder for power
 * - RFID Cards 13.56MHz Mifare Classic
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

// Pin definitions
#define RST_PIN 9
#define SS_PIN 10
#define BUZZER_PIN 3
#define NEOPIXEL_PIN 6
#define NEOPIXEL_COUNT 12

// Hardware setup
MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Game states - Press Your Luck mechanics
enum GameState {
  WAITING_FOR_CHALLENGE,
  SCANNING_TOKENS,
  GAME_COMPLETE
};

GameState currentState = WAITING_FOR_CHALLENGE;

// Press your luck variables
int currentScore = 0;
int tokensScanned = 0;
int correctScans = 0;
bool gameActive = false;

// Token database structure
struct Token {
  String uid;
  String name;
  String code;
  long gdp;      // GDP in billions
  int population; // Population in millions
  int area;      // Area in thousand km²
};

// Sample token database (expand this with your tokens)
Token tokens[] = {
  {"04 52 D6 A2", "United States", "USA", 21427, 331, 9834},
  {"04 7A B4 C2", "China", "CHN", 14342, 1439, 9597},
  {"04 3E F1 B2", "Japan", "JPN", 5081, 125, 378},
  {"04 2B C5 D3", "Germany", "GER", 3846, 84, 357},
  {"04 1F A7 E4", "India", "IND", 2875, 1380, 3287},
  {"04 6C B9 F5", "United Kingdom", "GBR", 2829, 67, 244},
  {"04 8D E2 A6", "France", "FRA", 2716, 67, 549},
  {"04 4A F3 B7", "Italy", "ITA", 2001, 60, 301},
  {"04 9E C8 D9", "Brazil", "BRA", 1869, 215, 8516},
  {"04 5B A4 E1", "Canada", "CAN", 1736, 38, 9985}
};

const int TOKEN_COUNT = sizeof(tokens) / sizeof(tokens[0]);

// Challenge types
enum ChallengeType {
  CHALLENGE_GDP,
  CHALLENGE_POPULATION,
  CHALLENGE_AREA,
  CHALLENGE_GDP_PER_CAPITA,
  CHALLENGE_NONE
};

struct Challenge {
  String uid;
  String name;
  ChallengeType type;
  bool highToLow;
};

// Challenge database
Challenge challenges[] = {
  {"04 A1 B2 C3", "GDP Challenge", CHALLENGE_GDP, true},
  {"04 D4 E5 F6", "Population Challenge", CHALLENGE_POPULATION, true},
  {"04 G7 H8 I9", "Area Challenge", CHALLENGE_AREA, true},
  {"04 J1 K2 L3", "GDP per Capita", CHALLENGE_GDP_PER_CAPITA, true}
};

const int CHALLENGE_COUNT = sizeof(challenges) / sizeof(challenges[0]);

// Game variables
String currentRanking[10];
String currentRankingCodes[10];
int rankingCount = 0;
ChallengeType currentChallenge = CHALLENGE_NONE;
bool challengeHighToLow = true;
String currentChallengeUID = "";
unsigned long lastScanTime = 0;
const unsigned long SCAN_DELAY = 1000; // 1 second between scans

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println(F("=== KNOW-IT-ALL PRESS YOUR LUCK PROTOTYPE ==="));
  
  // Initialize RFID
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Initialize NeoPixel ring
  pixels.begin();
  pixels.clear();
  pixels.show();
  
  // Initialize buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Welcome sequence
  playWelcomeSound();
  welcomeLightShow();
  
  Serial.println(F("🎮 PRESS YOUR LUCK SCANNING GAME"));
  Serial.println(F("Scan a challenge card to begin!"));
  printInstructions();
}

void loop() {
  // Check for RFID card
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Prevent rapid re-scanning
    if (millis() - lastScanTime > SCAN_DELAY) {
      String uid = getUID();
      handleCardScan(uid);
      lastScanTime = millis();
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
  
  // Check for button press (optional reveal trigger)
  if (digitalRead(BUTTON_PIN) == LOW && currentState == BUILDING_RANKING && rankingCount > 0) {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      currentState = REVEALING_RESULTS;
      startRevealSequence();
      while (digitalRead(BUTTON_PIN) == LOW); // Wait for release
    }
  }
  
  delay(100);
}

void handleCardScan(String uid) {
  Serial.print(F("📡 Scanned: "));
  Serial.println(uid);
  
  switch(currentState) {
    case WAITING_FOR_CHALLENGE:
      if (isChallengeCard(uid)) {
        loadChallenge(uid);
      } else {
        flashRed();
        playErrorBuzz();
        Serial.println(F("❌ Please scan a CHALLENGE CARD first!"));
      }
      break;
      
    case SCANNING_TOKENS:
      if (isChallengeCard(uid)) {
        // End game - show final score
        endGame();
      } else if (isToken(uid)) {
        // IMMEDIATE FEEDBACK - This is the press-your-luck moment!
        scanTokenImmediate(uid);
      } else {
        flashRed();
        playErrorBuzz();
        Serial.println(F("❌ Unknown card! Scan tokens or challenge card to finish."));
      }
      break;
      
    case GAME_COMPLETE:
      if (isChallengeCard(uid)) {
        resetGame();
        loadChallenge(uid);
      } else {
        Serial.println(F("🎮 Game complete! Scan a challenge card to play again."));
      }
      break;
  }
}

bool isChallengeCard(String uid) {
  for (int i = 0; i < CHALLENGE_COUNT; i++) {
    if (challenges[i].uid == uid) {
      return true;
    }
  }
  return false;
}

bool isToken(String uid) {
  for (int i = 0; i < TOKEN_COUNT; i++) {
    if (tokens[i].uid == uid) {
      return true;
    }
  }
  return false;
}

void loadChallenge(String uid) {
  for (int i = 0; i < CHALLENGE_COUNT; i++) {
    if (challenges[i].uid == uid) {
      currentChallenge = challenges[i].type;
      challengeHighToLow = challenges[i].highToLow;
      currentChallengeUID = uid;
      currentState = SCANNING_TOKENS;
      
      // Reset game variables
      currentScore = 0;
      tokensScanned = 0;
      correctScans = 0;
      gameActive = true;
      
      // 🎮 GAME EXPERIENCE: Challenge start audio/visual sequence
      playSuccessSound();
      challengeStartAnimation();
      
      // Debugging info (not part of game experience)
      Serial.println(F("🎯=================================🎯"));
      Serial.print(F("🎮 CHALLENGE: "));
      Serial.println(challenges[i].name);
      Serial.print(F("📊 Direction: "));
      Serial.println(challengeHighToLow ? "Highest to Lowest" : "Lowest to Highest");
      Serial.println(F("🎯=================================🎯"));
      Serial.println(F("🎲 PRESS YOUR LUCK! Start scanning tokens..."));
      return;
    }
  }
}

// ⭐ CORE PRESS-YOUR-LUCK FUNCTION ⭐
// Pure audio/visual feedback - no screen needed!
void scanTokenImmediate(String uid) {
  // Find the token
  Token* scannedToken = nullptr;
  for (int i = 0; i < TOKEN_COUNT; i++) {
    if (tokens[i].uid == uid) {
      scannedToken = &tokens[i];
      break;
    }
  }
  
  if (!scannedToken) {
    flashRed();
    playErrorBuzz();
    return;
  }
  
  tokensScanned++;
  
  // BUILD SUSPENSE - like real RFID scanner processing
  scanningAnimation();
  playScanningSound();
  delay(1200); // Build tension!
  
  // THE MOMENT OF TRUTH - Is it correct for this position?
  bool isCorrect = checkIfCorrectForPosition(scannedToken, tokensScanned);
  
  if (isCorrect) {
    // SUCCESS! 
    correctScans++;
    currentScore += (tokensScanned * 10);
    
    // AUDIO: Success pattern
    playCorrectBuzz();
    
    // VISUAL: Green celebration
    flashGreen();
    
    // Show current streak with LED pattern
    showStreakLEDs(correctScans);
    
  } else {
    // WRONG! LOSE EVERYTHING!
    gameActive = false;
    currentScore = 0;
    
    // AUDIO: Failure sound
    playWrongBuzz();
    
    // VISUAL: Red death flash
    flashRed();
    deathAnimation();
    
    currentState = GAME_COMPLETE;
  }
  
  if (gameActive) {
    // Visual indication: ready for next scan
    readyForNextScan();
  }
}

void startRevealSequence() {
  Serial.println(F("\n🎯 STARTING REVEAL SEQUENCE! 🎯"));
  playCountdownSound();
  
  // Get correct ranking
  String correctRanking[10];
  int correctCount = getCorrectRanking(correctRanking);
  
  Serial.println(F("\n=== REVEALING RESULTS ==="));
  
  int correctPositions = 0;
  
  for (int i = 0; i < rankingCount; i++) {
    delay(1500); // Dramatic pause
    
    String playerToken = currentRanking[i];
    String playerName = getTokenName(playerToken);
    String playerCode = getTokenCode(playerToken);
    
    Serial.print(F("Position "));
    Serial.print(i + 1);
    Serial.print(F(": "));
    Serial.print(playerName);
    Serial.print(F(" ("));
    Serial.print(playerCode);
    Serial.print(F(") - "));
    
    // Check if correct
    bool isCorrect = false;
    if (i < correctCount && correctRanking[i] == playerToken) {
      isCorrect = true;
      correctPositions++;
    }
    
    if (isCorrect) {
      Serial.println(F("✅ CORRECT!"));
      playCorrectSound();
    } else {
      Serial.print(F("❌ WRONG - Should be: "));
      if (i < correctCount) {
        String correctName = getTokenName(correctRanking[i]);
        String correctCode = getTokenCode(correctRanking[i]);
        Serial.print(correctName);
        Serial.print(F(" ("));
        Serial.print(correctCode);
        Serial.println(F(")"));
      } else {
        Serial.println(F("Not in top rankings"));
      }
      playIncorrectSound();
    }
    
    // Show the actual value
    showTokenValue(playerToken);
  }
  
  // Final score
  Serial.println(F("\n=== FINAL RESULTS ==="));
  Serial.print(F("Correct positions: "));
  Serial.print(correctPositions);
  Serial.print(F(" out of "));
  Serial.println(rankingCount);
  
  float percentage = (float)correctPositions / rankingCount * 100;
  Serial.print(F("Accuracy: "));
  Serial.print(percentage, 1);
  Serial.println(F("%"));
  
  if (percentage >= 80) {
    Serial.println(F("🏆 EXCELLENT! You know your stuff!"));
    playVictorySound();
  } else if (percentage >= 60) {
    Serial.println(F("👍 Good job! Room for improvement."));
    playGoodSound();
  } else {
    Serial.println(F("📚 Keep studying! Better luck next time."));
    playTryAgainSound();
  }
  
  currentState = GAME_COMPLETE;
  Serial.println(F("\nScan a new challenge card to play again!"));
}

int getCorrectRanking(String correctRanking[]) {
  // Create array of tokens that are in the current ranking
  Token rankingTokens[10];
  int count = 0;
  
  for (int i = 0; i < rankingCount; i++) {
    for (int j = 0; j < TOKEN_COUNT; j++) {
      if (tokens[j].uid == currentRanking[i]) {
        rankingTokens[count] = tokens[j];
        count++;
        break;
      }
    }
  }
  
  // Sort based on challenge type
  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      bool shouldSwap = false;
      
      switch (currentChallenge) {
        case CHALLENGE_GDP:
          shouldSwap = challengeHighToLow ? 
            (rankingTokens[j].gdp < rankingTokens[j+1].gdp) :
            (rankingTokens[j].gdp > rankingTokens[j+1].gdp);
          break;
          
        case CHALLENGE_POPULATION:
          shouldSwap = challengeHighToLow ? 
            (rankingTokens[j].population < rankingTokens[j+1].population) :
            (rankingTokens[j].population > rankingTokens[j+1].population);
          break;
          
        case CHALLENGE_AREA:
          shouldSwap = challengeHighToLow ? 
            (rankingTokens[j].area < rankingTokens[j+1].area) :
            (rankingTokens[j].area > rankingTokens[j+1].area);
          break;
          
        case CHALLENGE_GDP_PER_CAPITA:
          float gdpPerCapA = (float)rankingTokens[j].gdp / rankingTokens[j].population;
          float gdpPerCapB = (float)rankingTokens[j+1].gdp / rankingTokens[j+1].population;
          shouldSwap = challengeHighToLow ? 
            (gdpPerCapA < gdpPerCapB) :
            (gdpPerCapA > gdpPerCapB);
          break;
      }
      
      if (shouldSwap) {
        Token temp = rankingTokens[j];
        rankingTokens[j] = rankingTokens[j+1];
        rankingTokens[j+1] = temp;
      }
    }
  }
  
  // Convert back to UIDs
  for (int i = 0; i < count; i++) {
    correctRanking[i] = rankingTokens[i].uid;
  }
  
  return count;
}

void showTokenValue(String uid) {
  for (int i = 0; i < TOKEN_COUNT; i++) {
    if (tokens[i].uid == uid) {
      Serial.print(F("  → "));
      
      switch (currentChallenge) {
        case CHALLENGE_GDP:
          Serial.print(F("GDP: $"));
          Serial.print(tokens[i].gdp);
          Serial.println(F("B"));
          break;
          
        case CHALLENGE_POPULATION:
          Serial.print(F("Population: "));
          Serial.print(tokens[i].population);
          Serial.println(F("M"));
          break;
          
        case CHALLENGE_AREA:
          Serial.print(F("Area: "));
          Serial.print(tokens[i].area);
          Serial.println(F("k km²"));
          break;
          
        case CHALLENGE_GDP_PER_CAPITA:
          float gdpPerCap = (float)tokens[i].gdp * 1000 / tokens[i].population;
          Serial.print(F("GDP per capita: $"));
          Serial.print(gdpPerCap, 0);
          Serial.println();
          break;
      }
      break;
    }
  }
}

String getTokenName(String uid) {
  for (int i = 0; i < TOKEN_COUNT; i++) {
    if (tokens[i].uid == uid) {
      return tokens[i].name;
    }
  }
  return "Unknown";
}

String getTokenCode(String uid) {
  for (int i = 0; i < TOKEN_COUNT; i++) {
    if (tokens[i].uid == uid) {
      return tokens[i].code;
    }
  }
  return "???";
}

// Helper functions for press-your-luck mechanics
long getTokenValue(Token* token) {
  switch (currentChallenge) {
    case CHALLENGE_GDP:
      return token->gdp;
    case CHALLENGE_POPULATION:
      return token->population;
    case CHALLENGE_AREA:
      return token->area;
    case CHALLENGE_GDP_PER_CAPITA:
      return (long)((float)token->gdp * 1000 / token->population);
    default:
      return 0;
  }
}

void displayValue(long value) {
  switch (currentChallenge) {
    case CHALLENGE_GDP:
      Serial.print(F("$"));
      Serial.print(value);
      Serial.println(F(" billion"));
      break;
    case CHALLENGE_POPULATION:
      Serial.print(value);
      Serial.println(F(" million people"));
      break;
    case CHALLENGE_AREA:
      Serial.print(value);
      Serial.println(F(" thousand km²"));
      break;
    case CHALLENGE_GDP_PER_CAPITA:
      Serial.print(F("$"));
      Serial.print(value);
      Serial.println(F(" per person"));
      break;
  }
}

bool checkIfCorrectForPosition(Token* scannedToken, int position) {
  // For position 1, any token is "correct" (no comparison needed)
  if (position == 1) return true;
  
  // For position 2+, we need to check if it's in correct order relative to previous tokens
  // This is simplified - in real game, you'd track all previous tokens
  // For prototype, we'll make it progressively harder
  
  long tokenValue = getTokenValue(scannedToken);
  
  // Simulate checking against "expected" range for this position
  // Higher positions have stricter requirements
  if (challengeHighToLow) {
    // For highest-to-lowest, later positions should have lower values
    long expectedMax = 25000 / position; // Rough heuristic
    return tokenValue <= expectedMax;
  } else {
    // For lowest-to-highest, later positions should have higher values  
    long expectedMin = 1000 * position; // Rough heuristic
    return tokenValue >= expectedMin;
  }
}

void endGame() {
  if (gameActive && tokensScanned > 0) {
    // 🎮 GAME EXPERIENCE: Banking points celebration
    bankingAnimation();
    playVictoryBuzz();
    
    // Show final score with LED pattern
    showFinalScore(correctScans, tokensScanned);
    
    // Debugging info
    Serial.println(F("🏆 POINTS BANKED!"));
    Serial.print(F("💰 Final Score: "));
    Serial.println(currentScore);
    Serial.print(F("🎯 Streak: "));
    Serial.print(correctScans);
    Serial.print(F(" / "));
    Serial.println(tokensScanned);
    
  } else {
    // 🎮 GAME EXPERIENCE: No points to bank
    playErrorBuzz();
    flashRed();
    
    Serial.println(F("🏁 Game ended with no points to bank."));
  }
  
  currentState = GAME_COMPLETE;
  Serial.println(F("🎮 Scan a new challenge card to play again!"));
}

void resetGame() {
  currentState = WAITING_FOR_CHALLENGE;
  currentChallenge = CHALLENGE_NONE;
  currentChallengeUID = "";
  
  // Reset press-your-luck variables
  currentScore = 0;
  tokensScanned = 0;
  correctScans = 0;
  gameActive = false;
  
  pixels.clear();
  pixels.show();
  
  Serial.println(F("\n🎮 === GAME RESET === 🎮"));
  Serial.println(F("Scan a challenge card to begin new game!"));
}

String getUID() {
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content += (mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    content += String(mfrc522.uid.uidByte[i], HEX);
  }
  content.toUpperCase();
  return content.substring(1);
}

void printInstructions() {
  Serial.println(F("\n📋 HOW TO PLAY:"));
  Serial.println(F("1. Scan a CHALLENGE CARD to load a challenge"));
  Serial.println(F("2. Scan TOKENS in your predicted ranking order"));
  Serial.println(F("3. Scan the CHALLENGE CARD again to reveal results"));
  Serial.println(F("4. See how many you got right!"));
  Serial.println(F("\n🎮 Available Commands:"));
  Serial.println(F("- Type 'help' for instructions"));
  Serial.println(F("- Type 'tokens' to see available tokens"));
  Serial.println(F("- Type 'challenges' to see available challenges"));
  Serial.println(F(""));
}

// ===== SOUND & LIGHT FUNCTIONS =====

void buzz(int frequency, int duration) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

void playWelcomeSound() {
  buzz(100, 200);
  delay(100);
  buzz(100, 200);
  delay(100);
  buzz(100, 300);
}

void playSuccessSound() {
  buzz(100, 150);
  delay(50);
  buzz(100, 200);
}

void playErrorBuzz() {
  buzz(100, 800); // Long error buzz
}

void playCorrectBuzz() {
  // Happy success sound
  buzz(100, 100);
  delay(50);
  buzz(100, 100);
  delay(50);
  buzz(100, 200);
}

void playWrongBuzz() {
  // Sad failure sound
  buzz(100, 1000); // Long buzz of doom
}

void playVictoryBuzz() {
  // Banking points celebration - happy ascending pattern
  buzz(100, 100);
  delay(50);
  buzz(100, 100);
  delay(50);
  buzz(100, 100);
  delay(50);
  buzz(100, 200); // Longer final buzz
}

void playScanningSound() {
  // Rapid beeping during scan processing
  for (int i = 0; i < 8; i++) {
    buzz(100, 50);
    delay(80);
  }
}

// ===== NEOPIXEL ANIMATIONS =====

void welcomeLightShow() {
  // Rainbow spinner
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.clear();
      pixels.setPixelColor(i, pixels.Color(50, 0, 50)); // Purple
      pixels.show();
      delay(100);
    }
  }
  pixels.clear();
  pixels.show();
}

void challengeStartAnimation() {
  // Blue pulse to indicate challenge start
  for (int brightness = 0; brightness < 100; brightness += 10) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, brightness));
    }
    pixels.show();
    delay(50);
  }
  for (int brightness = 100; brightness >= 0; brightness -= 10) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, brightness));
    }
    pixels.show();
    delay(50);
  }
}

void scanningAnimation() {
  // Spinning yellow light while "scanning"
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.clear();
    pixels.setPixelColor(i, pixels.Color(100, 100, 0)); // Yellow
    pixels.setPixelColor((i + 1) % NEOPIXEL_COUNT, pixels.Color(50, 50, 0));
    pixels.show();
    delay(80);
  }
}

void flashGreen() {
  // SUCCESS! Flash green
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Bright green
  }
  pixels.show();
  delay(500);
  
  // Fade out
  for (int brightness = 255; brightness >= 0; brightness -= 15) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(0, brightness, 0));
    }
    pixels.show();
    delay(30);
  }
}

void flashRed() {
  // WRONG! Flash red
  for (int flash = 0; flash < 3; flash++) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Bright red
    }
    pixels.show();
    delay(200);
    
    pixels.clear();
    pixels.show();
    delay(200);
  }
}

void bankingAnimation() {
  // Banking points - gold spinning effect
  for (int spin = 0; spin < 24; spin++) {
    pixels.clear();
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      int brightness = (i + spin) % 4 == 0 ? 255 : 50;
      pixels.setPixelColor(i, pixels.Color(brightness, brightness, 0)); // Gold
    }
    pixels.show();
    delay(100);
  }
  
  // Final celebration flash
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 215, 0)); // Gold
  }
  pixels.show();
  delay(1000);
  
  pixels.clear();
  pixels.show();
}

// Additional visual feedback functions
void showStreakLEDs(int streak) {
  // Light up LEDs to show current streak (1-12 max)
  pixels.clear();
  for (int i = 0; i < min(streak, NEOPIXEL_COUNT); i++) {
    pixels.setPixelColor(i, pixels.Color(0, 100, 0)); // Green for streak
  }
  pixels.show();
  delay(1000);
  pixels.clear();
  pixels.show();
}

void deathAnimation() {
  // Death spiral - red spinning inward
  for (int round = 0; round < 3; round++) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.clear();
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.setPixelColor((NEOPIXEL_COUNT - 1 - i), pixels.Color(255, 0, 0));
      pixels.show();
      delay(100);
    }
  }
  pixels.clear();
  pixels.show();
}

void readyForNextScan() {
  // Gentle blue pulse indicating ready for next scan
  for (int brightness = 20; brightness <= 60; brightness += 10) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, brightness));
    }
    pixels.show();
    delay(100);
  }
  for (int brightness = 60; brightness >= 20; brightness -= 10) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, brightness));
    }
    pixels.show();
    delay(100);
  }
  pixels.clear();
  pixels.show();
}

void showFinalScore(int correct, int total) {
  // Show final score as percentage of LEDs lit
  pixels.clear();
  
  if (total > 0) {
    int ledsToLight = (correct * NEOPIXEL_COUNT) / total;
    
    for (int i = 0; i < ledsToLight; i++) {
      if (correct == total) {
        // Perfect game - gold
        pixels.setPixelColor(i, pixels.Color(255, 215, 0));
      } else if (correct >= total * 0.8) {
        // Good game - green
        pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      } else {
        // Okay game - blue
        pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      }
    }
  }
  
  pixels.show();
  delay(3000); // Show for 3 seconds
  pixels.clear();
  pixels.show();
}