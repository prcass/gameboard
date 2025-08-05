# GameBoard - RFID Press Your Luck Prototype

🎮 A physical prototype of an RFID-based "Press Your Luck" party game. Scan country tokens to test your knowledge - each scan gives immediate feedback and builds tension!

![Arduino RFID Game](https://img.shields.io/badge/Arduino-RFID%20Game-blue?style=for-the-badge&logo=arduino)
![Press Your Luck](https://img.shields.io/badge/Game%20Style-Press%20Your%20Luck-red?style=for-the-badge)
![Party Game](https://img.shields.io/badge/Category-Party%20Game-green?style=for-the-badge)

## 🎯 Game Overview

**GameBoard** simulates the natural "press your luck" tension of RFID scanning:

1. **Scan challenge card** → Blue pulse + beep (challenge loaded)
2. **Scan country token** → Yellow spinning + rapid beeps (processing...)
3. **Moment of truth** → Green flash + happy beep (CORRECT!) or Red flash + doom buzz (WRONG!)
4. **Decision point** → Continue scanning for more points or bank what you have

**Core Mechanic:** Each scan is an immediate moment of truth - no building rankings first, just pure press-your-luck excitement!

## 🛠️ Hardware Requirements

### Electronics (~$119 CAD)
- **Arduino Uno R3** - Main controller
- **IZOKEE MFRC522 RFID Kit** - 13.56MHz reader with test cards
- **12-LED NeoPixel Ring** - Visual feedback (WS2812B)
- **Elegoo Wire Kit** - Breadboard connections
- **Elegoo Breadboard 3-pack** - Prototyping base
- **Passive Buzzer Module** - Audio feedback
- **DKARDU 9V Battery Holders** - Portable power
- **PEUTIER NFC Stickers** - 50pc for tokens

### Game Materials
- Token bases (wooden discs, poker chips, etc.)
- Printed country labels
- Challenge cards (cardstock)
- Basic craft supplies

## 🚀 Quick Start

### 1. Arduino IDE Setup
```bash
# Install required libraries:
# - MFRC522 (RFID reader)
# - Adafruit NeoPixel (LED ring)
```

### 2. Upload Test Code
```cpp
// Upload component_test.ino first to verify all hardware works
// Then upload gameboard_prototype.ino for full game experience
```

### 3. Build Circuit
```
MFRC522 RFID → Arduino Uno
VCC → 3.3V (CRITICAL: NOT 5V!)
RST → Pin 9
SDA → Pin 10
// See EXACT_COMPONENTS_WIRING.md for complete connections
```

### 4. Create Tokens
- Apply NFC stickers to token bases
- Scan each token to record UID
- Update code with actual UIDs
- Apply country labels

## 📁 Project Structure

```
gameboard/
├── README.md                          # This file
├── arduino/
│   ├── gameboard_prototype.ino        # Main game code
│   ├── component_test.ino             # Hardware testing
│   └── uid_discovery.ino              # Token programming helper
├── hardware/
│   ├── EXACT_COMPONENTS_WIRING.md     # Wiring guide for purchased parts
│   ├── ASSEMBLY_CHECKLIST.md         # Step-by-step build guide
│   └── SHOPPING_LIST.md               # Component purchasing guide
├── game_materials/
│   ├── TOKEN_TEMPLATES.md             # Printable country labels
│   └── challenge_cards.pdf            # Printable challenge cards
└── docs/
    ├── QUICK_START_GUIDE.md           # Weekend build timeline
    └── TROUBLESHOOTING.md             # Common issues & fixes
```

## 🎮 Game Features

### Press-Your-Luck Mechanics
- **Immediate feedback** - Each scan gives instant audio/visual response
- **Risk/reward tension** - Continue scanning or bank your points
- **Escalating difficulty** - Later positions require more precision
- **Authentic RFID feel** - Just like scanning with a real RFID scanner

### Audio/Visual Feedback
- **12-LED NeoPixel animations** - Scanning, success, failure, banking
- **Buzzer sound patterns** - Processing, correct, wrong, victory
- **No screen required** - Pure audio/visual experience

### Party Game Features
- **Social gameplay** - Perfect for groups and parties
- **Competitive tension** - Risk vs reward on every scan
- **Quick rounds** - Fast-paced gameplay keeps everyone engaged
- **Real world trivia** - Country facts that spark conversation

## 🧪 Testing & Validation

### Component Test Results
- [ ] **Buzzer** - Clear audio feedback
- [ ] **NeoPixel Ring** - Smooth LED animations  
- [ ] **RFID Scanner** - Reliable card detection
- [ ] **Power System** - 4+ hour battery life

### Game Experience Validation
- [ ] **Scanning feels magical** - Instant response builds excitement
- [ ] **Press-your-luck tension** - Players genuinely torn between risk/reward
- [ ] **Party atmosphere** - Gets people laughing and competitive
- [ ] **Replayability** - Groups want multiple rounds

## 🎯 Success Metrics

**Technical Performance:**
- RFID scanning >95% success rate
- Sub-200ms response time
- 4+ hour battery life
- No crashes during normal play

**User Experience:**
- Players understand rules without lengthy explanation
- Natural tension builds with each additional scan
- Creates memorable party moments and reactions
- High replay desire across different groups

## 🛠️ Development Status

- [x] **Hardware design** - Complete component list and wiring
- [x] **Arduino firmware** - Press-your-luck game logic
- [x] **Audio/visual feedback** - NeoPixel animations and buzzer patterns
- [x] **Assembly guide** - Step-by-step build instructions
- [ ] **User testing** - Family/friend feedback sessions
- [ ] **Manufacturing assessment** - Cost and scalability analysis

## 🤝 Contributing

This is a prototype validation project. Feedback welcome on:
- User experience during gameplay
- Hardware reliability and durability
- Party game engagement and fun factor
- Manufacturing feasibility

## 📄 License

MIT License - Feel free to build, modify, and learn from this prototype!

## 🎪 About This Project

**GameBoard** is a proof-of-concept for a physical RFID-based party game. The goal is to validate the core "press your luck" scanning mechanic before investing in full-scale manufacturing.

Built with ❤️ using Arduino, RFID technology, and the excitement of social gaming.

---

**Ready to build your own GameBoard prototype? Check out the [Quick Start Guide](docs/QUICK_START_GUIDE.md)!** 🚀