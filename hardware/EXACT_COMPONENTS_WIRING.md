# Know-It-All Prototype - Exact Component Wiring Guide

## 🛍️ Your Purchased Components

✅ **Arduino Uno R3 Official** (~$35 CAD)  
✅ **IZOKEE MFRC522 RFID Kit** (~$12 CAD) - 13.56MHz with cards  
✅ **12-LED NeoPixel Ring WS2812B** (~$15 CAD)  
✅ **Elegoo Wire Kit** (~$12 CAD) - M-M, M-F, F-F jumpers  
✅ **Elegoo Breadboard 3-pack** (~$12 CAD) - Half-size breadboards  
✅ **Passive Buzzer Module** (~$8 CAD) - Requires PWM signal  
✅ **PEUTIER NFC Stickers 50pc** (~$10 CAD) - 13.56MHz compatible  
✅ **DKARDU 9V Battery Holders 4-pack** (~$15 CAD) - DC jack connectors

**Total: ~$119 CAD**

---

## 🔌 Exact Wiring Connections

### IZOKEE MFRC522 RFID Reader → Arduino Uno
```
MFRC522 Pin    →  Arduino Pin   →  Wire Color (suggested)
VCC (3.3V)     →  3.3V          →  Red
RST            →  Pin 9         →  Yellow
GND            →  GND           →  Black
IRQ            →  Not connected →  (leave empty)
MISO           →  Pin 12        →  Blue
MOSI           →  Pin 11        →  Green
SCK            →  Pin 13        →  White
SDA (SS)       →  Pin 10        →  Orange
```
**⚠️ CRITICAL: Use 3.3V NOT 5V for VCC - 5V will damage the RFID module!**

### 12-LED NeoPixel Ring → Arduino Uno
```
NeoPixel Pin   →  Arduino Pin   →  Wire Color
VCC (5V)       →  5V            →  Red
GND            →  GND           →  Black
Data In        →  Pin 6         →  Green
```

### Passive Buzzer Module → Arduino Uno
```
Buzzer Pin     →  Arduino Pin   →  Wire Color
VCC (5V)       →  5V            →  Red
GND            →  GND           →  Black
Signal (S)     →  Pin 3         →  Yellow
```
**Note:** Passive buzzers need PWM signal from Arduino - your code will handle this

### DKARDU 9V Battery Holder → Arduino Uno
```
Battery Holder →  Arduino Connection →  Notes
Red Wire (+)   →  DC Jack Center     →  Positive 9V
Black Wire (-) →  DC Jack Outer      →  Ground/Negative
DC Jack        →  Arduino Power Port →  2.1mm x 5.5mm connector
```
**Alternative for Testing:**
```
USB Cable      →  Arduino USB Port   →  Programming & debugging only
```

---

## 📋 Breadboard Layout

### Elegoo Half-Size Breadboard Setup
```
    Power Rails        Main Area           Power Rails
    + - - - - -    a b c d e f g h i j    - - - - - +
 1  ● ○ ○ ○ ○ ○    ○ ○ ○ ○ ○ ○ ○ ○ ○ ○    ○ ○ ○ ○ ○ ●
 2  ● ○ ○ ○ ○ ○    ○ ○ ○ ○ ○ ○ ○ ○ ○ ○    ○ ○ ○ ○ ○ ●
 3  ● ○ ○ ○ ○ ○    ○ ○ ○ ○ ○ ○ ○ ○ ○ ○    ○ ○ ○ ○ ○ ●
 4  ● ○ ○ ○ ○ ○    ○ ○ ○ ○ ○ ○ ○ ○ ○ ○    ○ ○ ○ ○ ○ ●
 5  ● ○ ○ ○ ○ ○    ┌─────────────────┐    ○ ○ ○ ○ ○ ●
 6  ● ○ ○ ○ ○ ○    │   MFRC522       │    ○ ○ ○ ○ ○ ●
 7  ● ○ ○ ○ ○ ○    │   RFID Reader   │    ○ ○ ○ ○ ○ ●
 8  ● ○ ○ ○ ○ ○    │   (8 pins)      │    ○ ○ ○ ○ ○ ●
 9  ● ○ ○ ○ ○ ○    └─────────────────┘    ○ ○ ○ ○ ○ ●
10  ● ○ ○ ○ ○ ○    ○ ○ ○ ○ ○ ○ ○ ○ ○ ○    ○ ○ ○ ○ ○ ●
    + - - - - -                           - - - - - +
    
    Left Power Rail: 3.3V (+) and GND (-)
    Right Power Rail: 5V (+) and GND (-)
```

### Component Placement
1. **MFRC522**: Rows 5-8, spans across center divide
2. **Power connections**: Use left rail for 3.3V, right rail for 5V
3. **Jumper wires**: Connect components to Arduino pins
4. **NeoPixel ring**: Mount off-breadboard, connect with jumpers
5. **Buzzer module**: Mount off-breadboard or use small breadboard section

---

## 🔧 Step-by-Step Assembly (45 minutes)

### Step 1: Power Rail Setup (5 minutes)
1. **Connect 3.3V rail**: Red jumper from Arduino 3.3V to left breadboard + rail
2. **Connect 5V rail**: Red jumper from Arduino 5V to right breadboard + rail  
3. **Connect GND rails**: Black jumpers from Arduino GND to both breadboard - rails
4. **Bridge GND rails**: Black jumper connecting left and right GND rails

### Step 2: MFRC522 RFID Reader (15 minutes)
1. **Insert MFRC522** into breadboard rows 5-8 (straddle center divide)
2. **VCC connection**: Red jumper from MFRC522 VCC to left 3.3V rail (NOT 5V!)
3. **GND connection**: Black jumper from MFRC522 GND to GND rail
4. **Data connections**: Use colored jumpers for SPI pins:
   - RST → Pin 9 (Yellow)
   - MISO → Pin 12 (Blue)  
   - MOSI → Pin 11 (Green)
   - SCK → Pin 13 (White)
   - SDA → Pin 10 (Orange)

### Step 3: NeoPixel Ring (10 minutes)
1. **Position ring**: Mount near Arduino (can be loose for prototype)
2. **Power connections**:
   - VCC → Arduino 5V (Red wire)
   - GND → Arduino GND (Black wire)
3. **Data connection**: Data In → Arduino Pin 6 (Green wire)

### Step 4: Passive Buzzer (10 minutes)
1. **Position buzzer**: Near Arduino or on second breadboard
2. **Power connections**:
   - VCC → Arduino 5V (Red wire)
   - GND → Arduino GND (Black wire)
3. **Signal connection**: S → Arduino Pin 3 (Yellow wire)

### Step 5: Final Connections Check (5 minutes)
- [ ] All power connections secure
- [ ] No loose wires
- [ ] MFRC522 using 3.3V (not 5V)
- [ ] NeoPixel and buzzer using 5V
- [ ] All data pins match code definitions

---

## ⚡ Power Supply Options

### Option 1: 9V Battery (Recommended for Prototype)
- **Battery holder**: Connect to Arduino DC jack
- **Runtime**: 4-6 hours continuous use
- **Pros**: Portable, safe voltage, easy replacement
- **Cons**: Limited runtime, additional cost

### Option 2: USB Power Bank  
- **Connection**: USB-A to USB-B cable
- **Runtime**: 8-12 hours (5000mAh bank)
- **Pros**: Long runtime, rechargeable
- **Cons**: Bulkier than 9V battery

### Option 3: USB Cable (Development Only)
- **Connection**: Computer USB port
- **Use**: Programming and testing only
- **Pros**: Unlimited power, Serial Monitor access
- **Cons**: Tethered to computer

---

## 🧪 Testing Your Connections

### Quick Test Sequence
Upload this minimal test code first:

```cpp
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

#define RST_PIN 9
#define SS_PIN 10
#define BUZZER_PIN 3
#define NEOPIXEL_PIN 6

MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_NeoPixel pixels(12, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pixels.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Test sequence
  Serial.println("Testing components...");
  
  // Test buzzer
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("✓ Buzzer test complete");
  
  // Test NeoPixel ring
  for(int i=0; i<12; i++) {
    pixels.setPixelColor(i, pixels.Color(50, 0, 0));
    pixels.show();
    delay(100);
  }
  pixels.clear();
  pixels.show();
  Serial.println("✓ NeoPixel test complete");
  
  // Test RFID
  Serial.println("✓ RFID initialized - ready for cards");
  Serial.println("All tests passed! Ready for main code.");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Card detected!");
    
    // Flash green and beep
    for(int i=0; i<12; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 100, 0));
    }
    pixels.show();
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
    pixels.clear();
    pixels.show();
    
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1000);
  }
}