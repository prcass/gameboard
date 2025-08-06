# GameBoard RFID Project - Session Context for Claude

## 🎮 Project Overview
Building a physical RFID-based "Press Your Luck" party game prototype using Arduino. Players scan country tokens to test knowledge with immediate feedback - each scan reveals CORRECT/WRONG instantly, creating natural tension about whether to continue or bank points.

## 📅 Current Session Status (August 6, 2025)

### ✅ Completed Tasks
1. **Hardware Assembly** - All components wired and tested
   - Arduino Uno R3 SMD version
   - IZOKEE MFRC522 RFID reader (13.56MHz) in column E, rows 15-22
   - 12-LED NeoPixel ring connected via male pins through holes
   - Passive buzzer module in rows 59-61
   - 9V battery holder connected
   - CRITICAL: RFID uses 3.3V (NOT 5V)

2. **Software Setup** 
   - Arduino IDE 2.3.6 installed
   - Libraries: MFRC522 and Adafruit NeoPixel
   - Component test passed - all hardware working

3. **Token Programming**
   - Scanned 10 country NFC stickers
   - Scanned 4 challenge card stickers
   - UIDs recorded in `scanned_uids.txt`

### 🔴 Current Issue
User trying to upload `gameboard_prototype.ino` but getting compilation errors. Created `gameboard_prototype_fixed.ino` with corrections but user hasn't uploaded it yet.

## 📁 File Locations
**User's files located at:**
```
C:\Users\rcass\Nest Wealth Dropbox\Randy Cass\PC\Documents\Arduino\gameboard-main\arduino\
```

**GitHub Repository:** https://github.com/prcass/gameboard

## 🏷️ Programmed UIDs

### Country Tokens (10 total)
```
Token #1 (USA): "1D 5B 00 76 0D 10 80"
Token #2 (China): "1D 0E 0E 76 0D 10 80"
Token #3 (Japan): "1D EC 10 76 0D 10 80"
Token #4 (Germany): "1D 05 1E 76 0D 10 80"
Token #5 (India): "1D 0D 21 76 0D 10 80"
Token #6 (United Kingdom): "1D 43 2E 76 0D 10 80"
Token #7 (France): "1D 23 31 76 0D 10 80"
Token #8 (Italy): "1D 30 3D 76 0D 10 80"
Token #9 (Brazil): "1D 39 40 76 0D 10 80"
Token #10 (Canada): "1D 42 4D 76 0D 10 80"
```

### Challenge Cards (4 total)
```
Challenge #1 (GDP): "1D C2 30 76 0D 10 80"
Challenge #2 (Population): "1D E1 4C 76 0D 10 80"
Challenge #3 (Area): "1D D8 3F 76 0D 10 80"
Challenge #4 (GDP per Capita): "1D D1 3C 76 0D 10 80"
```

## 🎯 Next Steps
1. **Upload Fixed Code** - User needs to upload `gameboard_prototype_fixed.ino`
2. **Test Gameplay** - Verify press-your-luck mechanics work
3. **Create Physical Tokens** - Apply stickers to bases, add labels
4. **Final Testing** - Full gameplay session

## 🔧 Hardware Configuration
- **RFID Module**: Column E, rows 15-22 (SDA at row 15)
- **Power Rails**: Left = 3.3V, Right = 5V
- **Connections**:
  - RFID SDA (e15) → Arduino Pin 10
  - RFID SCK (e16) → Arduino Pin 13
  - RFID MOSI (e17) → Arduino Pin 11
  - RFID MISO (e18) → Arduino Pin 12
  - RFID RST (e21) → Arduino Pin 9
  - RFID VCC (e22) → 3.3V
  - NeoPixel Data → Arduino Pin 6
  - Buzzer Signal → Arduino Pin 3

## 🎮 Game Mechanics
- **Press Your Luck**: Each token scan gives immediate feedback
- **Scoring**: Points increase with streak (position × 10)
- **Risk**: Wrong answer loses ALL points
- **Banking**: Scan challenge card again to keep points

## 📝 Important Notes
- User has Arduino Uno Rev3 SMD version (functionally identical)
- Using Arduino IDE 2.3.6 (Serial Monitor is a tab at bottom)
- Fixed code removes button references and corrects function names
- All 14 NFC stickers programmed and ready

## 🚀 To Resume Next Session
1. Check if `gameboard_prototype_fixed.ino` was uploaded successfully
2. If not, help upload the fixed code
3. Test complete game flow with physical scanning
4. Guide through creating token labels and bases
5. Document any gameplay adjustments needed

**Project Status:** 90% complete - just need to upload working code and create physical game pieces!