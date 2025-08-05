# DIY Token and Challenge Card Templates

## 🎯 Token Design Templates (25mm Diameter)

### Country Tokens - Print on Adhesive Labels

```
┌─────────────────────────┐
│         🇺🇸 USA         │
│    United States        │
│   GDP: $21.4 Trillion  │
│   Pop: 331 Million     │
└─────────────────────────┘

┌─────────────────────────┐
│         🇨🇳 CHN         │
│         China           │
│   GDP: $14.3 Trillion  │
│   Pop: 1.44 Billion    │
└─────────────────────────┘

┌─────────────────────────┐
│         🇯🇵 JPN         │
│         Japan           │
│   GDP: $5.1 Trillion   │
│   Pop: 125 Million     │
└─────────────────────────┘

┌─────────────────────────┐
│         🇩🇪 GER         │
│        Germany          │
│   GDP: $3.8 Trillion   │
│   Pop: 84 Million      │
└─────────────────────────┘

┌─────────────────────────┐
│         🇮🇳 IND         │
│         India           │
│   GDP: $2.9 Trillion   │
│   Pop: 1.38 Billion    │
└─────────────────────────┘

┌─────────────────────────┐
│         🇬🇧 GBR         │
│    United Kingdom       │
│   GDP: $2.8 Trillion   │
│   Pop: 67 Million      │
└─────────────────────────┘

┌─────────────────────────┐
│         🇫🇷 FRA         │
│        France           │
│   GDP: $2.7 Trillion   │
│   Pop: 67 Million      │
└─────────────────────────┘

┌─────────────────────────┐
│         🇮🇹 ITA         │
│         Italy           │
│   GDP: $2.0 Trillion   │
│   Pop: 60 Million      │
└─────────────────────────┘

┌─────────────────────────┐
│         🇧🇷 BRA         │
│        Brazil           │
│   GDP: $1.9 Trillion   │
│   Pop: 215 Million     │
└─────────────────────────┘

┌─────────────────────────┐
│         🇨🇦 CAN         │
│        Canada           │
│   GDP: $1.7 Trillion   │
│   Pop: 38 Million      │
└─────────────────────────┘
```

## 🎲 Challenge Card Templates (Playing Card Size)

### Challenge Card Fronts

```
┌─────────────────────────┐
│    🌍 GDP CHALLENGE     │
│                         │
│   Rank by Total GDP     │
│  (Highest to Lowest)    │
│                         │
│  💰 Gross Domestic      │
│     Product Value       │
│                         │
│      Challenge #1       │
└─────────────────────────┘

┌─────────────────────────┐
│  👥 POPULATION CHALLENGE│
│                         │
│  Rank by Population     │
│  (Highest to Lowest)    │
│                         │
│  🏙️ Total People        │
│     in Country          │
│                         │
│      Challenge #2       │
└─────────────────────────┘

┌─────────────────────────┐
│   🗺️ AREA CHALLENGE     │
│                         │
│  Rank by Land Area      │
│  (Highest to Lowest)    │
│                         │
│  📏 Square Kilometers   │
│     of Territory        │
│                         │
│      Challenge #3       │
└─────────────────────────┘

┌─────────────────────────┐
│ 💎 GDP PER CAPITA       │
│      CHALLENGE          │
│                         │
│  Rank by GDP per Person │
│  (Highest to Lowest)    │
│                         │
│  💵 Economic Wealth     │
│     per Citizen         │
│                         │
│      Challenge #4       │
└─────────────────────────┘
```

## 🏷️ RFID Tag Programming Guide

### UID Assignment Chart
**IMPORTANT:** After assembling tokens, scan each one to record its actual UID, then update the Arduino code.

```
Token Programming Log:
┌──────────────┬─────────────────┬────────────────┐
│   Country    │  Actual UID     │  Code Variable │
├──────────────┼─────────────────┼────────────────┤
│  USA         │ ____________    │  tokens[0].uid │
│  China       │ ____________    │  tokens[1].uid │
│  Japan       │ ____________    │  tokens[2].uid │
│  Germany     │ ____________    │  tokens[3].uid │
│  India       │ ____________    │  tokens[4].uid │
│  UK          │ ____________    │  tokens[5].uid │
│  France      │ ____________    │  tokens[6].uid │
│  Italy       │ ____________    │  tokens[7].uid │
│  Brazil      │ ____________    │  tokens[8].uid │
│  Canada      │ ____________    │  tokens[9].uid │
├──────────────┼─────────────────┼────────────────┤
│ Challenge 1  │ ____________    │ challenges[0]  │
│ Challenge 2  │ ____________    │ challenges[1]  │  
│ Challenge 3  │ ____________    │ challenges[2]  │
│ Challenge 4  │ ____________    │ challenges[3]  │
└──────────────┴─────────────────┴────────────────┘
```

### UID Discovery Process
1. **Upload the main sketch** to Arduino
2. **Open Serial Monitor** (9600 baud)
3. **Scan each token individually**
4. **Record the displayed UID** (format: "04 52 D6 A2")
5. **Update the code** with actual UIDs
6. **Re-upload** the corrected sketch

### Programming Helper Sketch
Upload this first to discover UIDs easily:

```cpp
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID UID Discovery Tool");
  Serial.println("Scan tokens to discover UIDs:");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1000);
  }
}
```

## 🎨 Assembly Instructions

### Token Assembly (30 minutes)
1. **Prepare wooden discs** - sand lightly if rough
2. **Apply RFID stickers** to back center of each disc
3. **Print token labels** on adhesive paper (Avery 22817 works well)
4. **Cut out circular labels** - use 1" circle punch or scissors
5. **Apply labels to front** - center carefully
6. **Test each token** - scan to verify RFID works

### Challenge Card Assembly (15 minutes)  
1. **Print challenge fronts** on cardstock or photo paper
2. **Cut to playing card size** (2.5" x 3.5")
3. **Apply small RFID stickers** to back corner (use smaller 13mm stickers)
4. **Laminate** for durability (optional but recommended)
5. **Test each card** - scan to verify RFID works

### Quality Control Checklist
- [ ] All tokens scan reliably from 2-3cm distance
- [ ] Token labels are centered and adhesive holds well
- [ ] Challenge card text is readable and clear
- [ ] RFID stickers don't interfere with stacking
- [ ] No duplicate UIDs detected
- [ ] All UIDs recorded and updated in code

## 📐 Foam Board Game Layout

### Game Board Template (12" x 16")

```
┌────────────────────────────────────────────────────────┐
│                    DIY KNOW-IT-ALL                     │
│                      PROTOTYPE                         │
├────────────────────────────────────────────────────────┤
│                                                        │
│  RANKING POSITIONS (Place tokens here in order)       │
│  ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐    │
│  │ 1 │ │ 2 │ │ 3 │ │ 4 │ │ 5 │ │ 6 │ │ 7 │ │ 8 │    │
│  └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘    │
│                                                        │
│                 ┌─────────────────┐                    │
│                 │                 │                    │
│   AVAILABLE     │  RFID SCANNER   │    INSTRUCTIONS   │
│    TOKENS       │     AREA        │                   │
│  ┌───┐ ┌───┐    │                 │  1. Scan Challenge│
│  │🇺🇸│ │🇨🇳│    │ (Arduino placed │  2. Scan Tokens   │
│  └───┘ └───┘    │  underneath)    │  3. Scan Challenge│
│  ┌───┐ ┌───┐    │                 │     Again         │
│  │🇯🇵│ │🇩🇪│    └─────────────────┘  4. See Results!  │
│  └───┘ └───┘                                          │
│  ┌───┐ ┌───┐      CHALLENGE CARDS                     │
│  │🇮🇳│ │🇬🇧│    ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐     │
│  └───┘ └───┘    │ GDP │ │POP. │ │AREA │ │$/PP │     │
│  ┌───┐ ┌───┐    └─────┘ └─────┘ └─────┘ └─────┘     │
│  │🇫🇷│ │🇮🇹│                                         │
│  └───┘ └───┘                                         │
│  ┌───┐ ┌───┐                                         │
│  │🇧🇷│ │🇨🇦│                                         │
│  └───┘ └───┘                                         │
└────────────────────────────────────────────────────────┘
```

### Cutting Guide
1. **Main board**: 12" x 16" foam board
2. **RFID scanner hole**: 4" x 3" rectangle in center
3. **Position markers**: 1" circles for ranking positions
4. **Token storage**: 1.2" circles for available tokens
5. **Card holders**: Small rectangular indentations (optional)

### Assembly Tips
- **Use craft knife** for clean cuts in foam board
- **Mark with pencil first** - measure twice, cut once
- **Sand edges smooth** to prevent snags
- **Add labels** with printed adhesive paper
- **Test fit components** before final assembly

## 🎪 Game Enhancement Ideas

### Visual Improvements
- **LED strip** around the ranking positions
- **Colored zones** for different challenge types
- **Score tracking area** with dry erase surface
- **Timer display** for competitive play

### Audio Enhancements  
- **Micro SD card** with recorded announcements
- **Volume control** knob or button
- **Different sound themes** selectable by button
- **Victory music** for perfect scores

### Gameplay Variations
- **Team mode** - multiple players collaborate
- **Speed rounds** - timer pressure
- **Bonus challenges** - double points for specific tokens
- **Tournament bracket** - elimination rounds

This prototype will give you a working proof-of-concept for under $100!