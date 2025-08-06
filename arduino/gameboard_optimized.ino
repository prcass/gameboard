/*
 * Know-It-All Press Your Luck RFID Game - OPTIMIZED VERSION
 * Fixed RFID reliability issues with better state management
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

// Game states
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
  long gdp;
  int population;
  int area;
};

// Token database - Your actual UIDs
Token tokens[] = {
  {"1D 5B 00 76 0D 10 80", "United States", "USA", 21427, 331, 9834},
  {"1D 0E 0E 76 0D 10 80", "China", "CHN", 14342, 1439, 9597},
  {"1D EC 10 76 0D 10 80", "Japan", "JPN", 5081, 125, 378},
  {"1D 05 1E 76 0D 10 80", "Germany", "GER", 3846, 84, 357},
  {"1D 0D 21 76 0D 10 80", "India", "IND", 2875, 1380, 3287},
  {"1D 43 2E 76 0D 10 80", "United Kingdom", "GBR", 2829, 67, 244},
  {"1D 23 31 76 0D 10 80", "France", "FRA", 2716, 67, 549},
  {"1D 30 3D 76 0D 10 80", "Italy", "ITA", 2001, 60, 301},
  {"1D 39 40 76 0D 10 80", "Brazil", "BRA", 1869, 215, 8516},
  {"1D 42 4D 76 0D 10 80", "Canada", "CAN", 1736, 38, 9985}
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

// Challenge database - Your actual UIDs
Challenge challenges[] = {
  {"1D C2 30 76 0D 10 80", "GDP Challenge", CHALLENGE_GDP, true},
  {"1D E1 4C 76 0D 10 80", "Population Challenge", CHALLENGE_POPULATION, true},
  {"1D D8 3F 76 0D 10 80", "Area Challenge", CHALLENGE_AREA, true},
  {"1D D1 3C 76 0D 10 80", "GDP per Capita", CHALLENGE_GDP_PER_CAPITA, true}
};

const int CHALLENGE_COUNT = sizeof(challenges) / sizeof(challenges[0]);

// Game variables
ChallengeType currentChallenge = CHALLENGE_NONE;
bool challengeHighToLow = true;
String currentChallengeUID = "";
unsigned long lastScanTime = 0;
const unsigned long SCAN_DELAY = 2000; // Increased to 2 seconds
String lastScannedUID = ""; // Prevent duplicate scans

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println(F("=== OPTIMIZED GAMEBOARD PROTOTYPE ==="));
  
  // Initialize RFID with error checking
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Test RFID initialization
  if (!mfrc522.PCD_PerformSelfTest()) {
    Serial.println(F("⚠️  RFID self-test failed!"));
  } else {
    Serial.println(F("✅ RFID initialized successfully"));
  }
  
  // Re-initialize after self test
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
  Serial.println(F(""));
}

void loop() {
  // IMPROVED RFID SCANNING LOGIC
  
  // Reset RFID periodically to prevent lockups
  static unsigned long lastReset = 0;
  if (millis() - lastReset > 10000) { // Reset every 10 seconds
    mfrc522.PCD_Init();
    lastReset = millis();
  }
  
  // Check for new card with better error handling
  if (!mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  
  if (!mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
  
  // Get UID
  String uid = getUID();
  
  // Prevent rapid re-scanning of same card
  if (millis() - lastScanTime < SCAN_DELAY || uid == lastScannedUID) {
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(50);
    return;
  }
  
  // Process the card
  Serial.print(F("📡 Scanned: "));
  Serial.println(uid);
  
  handleCardScan(uid);
  
  // Update tracking variables
  lastScanTime = millis();
  lastScannedUID = uid;
  
  // Properly halt card communication
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  // Short delay to ensure clean state
  delay(100);
}

void handleCardScan(String uid) {
  switch(currentState) {
    case WAITING_FOR_CHALLENGE:
      if (isChallengeCard(uid)) {
        loadChallenge(uid);
      } else if (isToken(uid)) {
        flashRed();
        playErrorBuzz();
        Serial.println(F("❌ Please scan a CHALLENGE CARD first!"));
      } else {
        flashRed();
        playErrorBuzz();
        Serial.print(F("❌ Unknown card UID: "));
        Serial.print(uid);
        Serial.println(F(" - Please scan a challenge card."));
      }
      break;
      
    case SCANNING_TOKENS:
      if (isChallengeCard(uid)) {
        endGame();
      } else if (isToken(uid)) {
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
  Serial.print(F("🔍 Checking UID: '"));
  Serial.print(uid);
  Serial.print(F("' (length: "));
  Serial.print(uid.length());
  Serial.println(F(")"));
  
  for (int i = 0; i < CHALLENGE_COUNT; i++) {
    Serial.print(F("   vs Challenge "));
    Serial.print(i);
    Serial.print(F(": '"));
    Serial.print(challenges[i].uid);
    Serial.println(F("'"));
    
    if (challenges[i].uid.equals(uid)) {  // Use .equals() instead of ==
      Serial.println(F("✅ MATCH FOUND!"));
      return true;
    }
  }
  Serial.println(F("❌ NO MATCH"));
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
      
      // Clear last scanned to allow immediate token scanning
      lastScannedUID = "";
      lastScanTime = millis() - SCAN_DELAY;
      
      playSuccessSound();
      challengeStartAnimation();
      
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

void scanTokenImmediate(String uid) {
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
    Serial.println(F("❌ Token not found in database!"));
    return;
  }
  
  tokensScanned++;
  
  // Build suspense
  scanningAnimation();
  playScanningSound();
  delay(800); // Reduced from 1200
  
  // Check if correct
  bool isCorrect = checkIfCorrectForPosition(scannedToken, tokensScanned);
  
  if (isCorrect) {
    correctScans++;
    currentScore += (tokensScanned * 10);
    
    playCorrectBuzz();
    flashGreen();
    showStreakLEDs(correctScans);
    
    Serial.print(F("✅ CORRECT! "));
    Serial.print(scannedToken->name);
    Serial.print(F(" - Score: "));
    Serial.println(currentScore);
    Serial.println(F("🎲 Scan next token or challenge card to cash out!"));
    
  } else {
    gameActive = false;
    currentScore = 0;
    
    playWrongBuzz();
    flashRed();
    deathAnimation();
    
    Serial.print(F("❌ WRONG! "));
    Serial.print(scannedToken->name);
    Serial.println(F(" - GAME OVER!"));
    
    currentState = GAME_COMPLETE;
    Serial.println(F("🎮 Scan a new challenge card to play again!"));
  }
}

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

bool checkIfCorrectForPosition(Token* scannedToken, int position) {
  if (position == 1) return true;
  
  long tokenValue = getTokenValue(scannedToken);
  
  if (challengeHighToLow) {
    long expectedMax = 25000 / position;
    return tokenValue <= expectedMax;
  } else {
    long expectedMin = 1000 * position;
    return tokenValue >= expectedMin;
  }
}

void endGame() {
  if (gameActive && tokensScanned > 0) {
    bankingAnimation();
    playVictoryBuzz();
    showFinalScore(correctScans, tokensScanned);
    
    Serial.println(F("🏆 POINTS BANKED!"));
    Serial.print(F("💰 Final Score: "));
    Serial.println(currentScore);
    Serial.print(F("🎯 Streak: "));
    Serial.print(correctScans);
    Serial.print(F(" / "));
    Serial.println(tokensScanned);
  } else {
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
  
  currentScore = 0;
  tokensScanned = 0;
  correctScans = 0;
  gameActive = false;
  
  // Clear scan tracking
  lastScannedUID = "";
  lastScanTime = 0;
  
  pixels.clear();
  pixels.show();
  
  Serial.println(F("\n🎮 === GAME RESET === 🎮"));
  Serial.println(F("Scan a challenge card to begin new game!"));
}

String getUID() {
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (content.length() > 0) content += " ";  // Add space between bytes
    if (mfrc522.uid.uidByte[i] < 0x10) content += "0";  // Leading zero
    content += String(mfrc522.uid.uidByte[i], HEX);
  }
  content.toUpperCase();
  return content;
}

// Sound functions
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
  buzz(100, 600);
}

void playCorrectBuzz() {
  buzz(100, 100);
  delay(50);
  buzz(100, 100);
  delay(50);
  buzz(100, 200);
}

void playWrongBuzz() {
  buzz(100, 800);
}

void playVictoryBuzz() {
  buzz(100, 100);
  delay(50);
  buzz(100, 100);
  delay(50);
  buzz(100, 100);
  delay(50);
  buzz(100, 200);
}

void playScanningSound() {
  for (int i = 0; i < 6; i++) {
    buzz(100, 40);
    delay(60);
  }
}

// Light functions
void welcomeLightShow() {
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.clear();
      pixels.setPixelColor(i, pixels.Color(50, 0, 50));
      pixels.show();
      delay(80);
    }
  }
  pixels.clear();
  pixels.show();
}

void challengeStartAnimation() {
  for (int brightness = 0; brightness < 100; brightness += 20) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, brightness));
    }
    pixels.show();
    delay(30);
  }
  for (int brightness = 100; brightness >= 0; brightness -= 20) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, brightness));
    }
    pixels.show();
    delay(30);
  }
}

void scanningAnimation() {
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.clear();
    pixels.setPixelColor(i, pixels.Color(100, 100, 0));
    pixels.setPixelColor((i + 1) % NEOPIXEL_COUNT, pixels.Color(50, 50, 0));
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
  
  for (int brightness = 255; brightness >= 0; brightness -= 20) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.setPixelColor(i, pixels.Color(0, brightness, 0));
    }
    pixels.show();
    delay(20);
  }
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
  for (int spin = 0; spin < 16; spin++) {
    pixels.clear();
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      int brightness = (i + spin) % 4 == 0 ? 255 : 50;
      pixels.setPixelColor(i, pixels.Color(brightness, brightness, 0));
    }
    pixels.show();
    delay(80);
  }
  
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 215, 0));
  }
  pixels.show();
  delay(800);
  
  pixels.clear();
  pixels.show();
}

void showStreakLEDs(int streak) {
  pixels.clear();
  for (int i = 0; i < min(streak, NEOPIXEL_COUNT); i++) {
    pixels.setPixelColor(i, pixels.Color(0, 100, 0));
  }
  pixels.show();
  delay(800);
  pixels.clear();
  pixels.show();
}

void deathAnimation() {
  for (int round = 0; round < 2; round++) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      pixels.clear();
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.setPixelColor((NEOPIXEL_COUNT - 1 - i), pixels.Color(255, 0, 0));
      pixels.show();
      delay(80);
    }
  }
  pixels.clear();
  pixels.show();
}

void showFinalScore(int correct, int total) {
  pixels.clear();
  
  if (total > 0) {
    int ledsToLight = (correct * NEOPIXEL_COUNT) / total;
    
    for (int i = 0; i < ledsToLight; i++) {
      if (correct == total) {
        pixels.setPixelColor(i, pixels.Color(255, 215, 0));
      } else if (correct >= total * 0.8) {
        pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      } else {
        pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      }
    }
  }
  
  pixels.show();
  delay(2000);
  pixels.clear();
  pixels.show();
}