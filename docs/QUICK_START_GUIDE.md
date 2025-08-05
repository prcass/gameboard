# Know-It-All DIY Prototype - Quick Start Guide

## 🚀 One Weekend Build Plan

### ⏰ Timeline Overview
- **Friday Evening** (1 hour): Order components, gather tools
- **Saturday Morning** (3 hours): Build electronics, test connections
- **Saturday Afternoon** (3 hours): Create tokens, program RFID tags
- **Sunday Morning** (2 hours): Assemble game board, final testing
- **Sunday Afternoon** (1 hour): First gameplay session!

**Total Time: 10 hours over 3 days**  
**Total Cost: Under $100**

---

## 📋 Phase 1: Component Setup (Friday Evening)

### Pre-Build Checklist
- [ ] All components ordered/acquired (see SHOPPING_LIST.md)
- [ ] Arduino IDE installed on computer
- [ ] RFID library installed (`Tools > Manage Libraries > Search "MFRC522"`)
- [ ] USB cable connects Arduino to computer
- [ ] Serial Monitor working (9600 baud)

### Quick Connection Test (10 minutes)
1. **Connect Arduino to computer via USB**
2. **Upload blink sketch** (`File > Examples > 01.Basics > Blink`)
3. **Verify LED blinks** - confirms Arduino works
4. **Open Serial Monitor** - confirms communication

**Success Criteria:** Arduino LED blinks, Serial Monitor displays text

---

## 🔧 Phase 2: Electronics Build (Saturday Morning)

### Step 1: RFID Circuit (45 minutes)
Follow WIRING_GUIDE.md for complete connections:

**Critical Connections:**
```
MFRC522 → Arduino
VCC → 3.3V (NOT 5V!)
RST → Pin 9
GND → GND  
SDA → Pin 10
```

### Step 2: Speaker Circuit (15 minutes)
```
Speaker → Arduino
Positive → Pin 3 (through 220Ω resistor)
Negative → GND
```

### Step 3: First Test (30 minutes)
1. **Upload RFID discovery sketch** (from TOKEN_TEMPLATES.md)
2. **Scan RFID cards** that came with kit
3. **Record UIDs** displayed in Serial Monitor
4. **Test speaker** - should hear beep tones

**Success Criteria:** RFID cards scan reliably, speaker produces clear tones

### Step 4: Upload Main Game Code (30 minutes)
1. **Copy `know_it_all_prototype.ino`** from files created
2. **Update UIDs** in token database with your actual card UIDs
3. **Upload to Arduino**
4. **Test basic functions** - should show "Ready!" message

### Step 5: Power Test (15 minutes)
1. **Connect power bank** instead of USB
2. **Verify game boots up** independently
3. **Test battery life** - estimate 6-8 hours minimum

**Success Criteria:** Game runs independently on battery power

---

## 🎨 Phase 3: Game Tokens (Saturday Afternoon)

### Step 1: Physical Token Assembly (45 minutes)
1. **Apply RFID stickers** to backs of wooden discs
2. **Print token labels** on adhesive paper (see TOKEN_TEMPLATES.md)
3. **Cut circular labels** - 1" circle punch or careful scissors
4. **Apply labels to fronts** - center carefully
5. **Test each token** - scan to verify RFID function

### Step 2: UID Programming (60 minutes)
1. **Scan each token** with discovery sketch
2. **Record actual UIDs** in programming log
3. **Update main sketch** with correct UIDs
4. **Re-upload game code**
5. **Test each token** - should display correct country name

### Step 3: Challenge Cards (30 minutes)  
1. **Print challenge cards** on cardstock
2. **Cut to playing card size** (2.5" x 3.5")
3. **Apply small RFID stickers** to corners
4. **Scan and record UIDs**
5. **Update challenge database** in code

### Step 4: Final Programming Test (15 minutes)
1. **Upload complete game** with all correct UIDs
2. **Test each token** - should display country and stats
3. **Test each challenge** - should load correctly
4. **Verify game flow** - challenge → tokens → reveal

**Success Criteria:** All tokens and challenges recognized by name

---

## 🏗️ Phase 4: Game Board (Sunday Morning)

### Step 1: Board Layout (30 minutes)
1. **Cut foam board** to 12" x 16"
2. **Mark ranking positions** 1-8 with pencil
3. **Cut RFID scanner hole** 4" x 3" in center
4. **Mark token storage areas** around edges

### Step 2: Component Mounting (45 minutes)
1. **Mount Arduino underneath** board (tape or hot glue)
2. **Position RFID reader** in scanner hole
3. **Secure speaker** under board for best sound projection
4. **Route power bank cable** cleanly to side

### Step 3: Finishing Touches (30 minutes)
1. **Add position labels** - print numbers 1-8 on small circles
2. **Create instruction card** - basic gameplay steps
3. **Make token holders** - small circles or indentations
4. **Test component access** - ensure everything reachable

### Step 4: Final Assembly Test (15 minutes)
1. **Power on complete system**
2. **Test RFID reading** through board material
3. **Check speaker volume** and clarity
4. **Verify all tokens scan** reliably from gameplay position

**Success Criteria:** Complete system works as intended, professional appearance

---

## 🎮 Phase 5: First Gameplay (Sunday Afternoon)

### Gameplay Test Protocol

#### Test 1: Basic Flow (15 minutes)
1. **Scan GDP challenge card** - should load challenge
2. **Scan 3 country tokens** in order - should confirm each
3. **Scan challenge card again** - should start reveal sequence
4. **Watch automated reveal** - should show correct/incorrect for each position

#### Test 2: Different Challenge (10 minutes)
1. **Try population challenge** 
2. **Use different tokens**
3. **Verify correct ordering** matches expected results

#### Test 3: Error Handling (10 minutes)
- **Scan unknown card** - should give error message
- **Scan same token twice** - should reject duplicate
- **Try to start without challenge** - should prompt for challenge

#### Test 4: Family/Friend Test (15 minutes)
- **Explain rules** to someone else
- **Let them play** while you observe
- **Note confusion points** and usability issues
- **Get feedback** on fun factor and clarity

### Success Metrics Evaluation

**Technical Performance:**
- [ ] RFID scanning works reliably (>95% success rate)
- [ ] Audio feedback is clear and helpful
- [ ] Game flow is intuitive and logical
- [ ] No crashes or error states during normal play

**User Experience:**
- [ ] Players understand rules without extensive explanation
- [ ] Scanning tokens feels "magical" and responsive
- [ ] Reveal sequence is exciting and suspenseful  
- [ ] Players want to try again with different challenges

**Educational Value:**
- [ ] Players learn new facts about countries
- [ ] Prompts discussion about geography and economics
- [ ] Encourages curiosity about world data
- [ ] Memorable experience that sticks with players

---

## 🔧 Troubleshooting Quick Fixes

### RFID Not Working
**Symptom:** Cards don't scan or inconsistent reading  
**Fix:** Check 3.3V connection, ensure cards are 125kHz, try different cards

### No Sound
**Symptom:** Silent operation, no beeps  
**Fix:** Check speaker polarity, verify 220Ω resistor, test with multimeter

### Arduino Won't Boot
**Symptom:** No startup message, Serial Monitor blank  
**Fix:** Check power connections, try different USB cable, press reset button

### Wrong Country Names
**Symptom:** Scans show "Unknown" or wrong country  
**Fix:** Re-scan tokens to get actual UIDs, update code, re-upload

### Power Issues
**Symptom:** System resets randomly, poor performance  
**Fix:** Check power bank capacity, ensure 2A output, reduce LED brightness

---

## 🎯 Next Steps After Successful Build

### Immediate Improvements (Same Weekend)
- **Add more tokens** - expand to 15-20 countries
- **Create more challenges** - GDP per capita, land area, population density
- **Improve game board** - better labels, cleaner layout
- **Test with more people** - gather feedback from different age groups

### Week 2 Enhancements
- **LED feedback** - visual confirmation of scans
- **Better audio** - recorded announcements vs simple beeps
- **Score tracking** - keep track of accuracy across games
- **Tournament mode** - bracket-style competition

### Month 2: Advanced Features
- **Bluetooth connectivity** - connect to phone app
- **Online leaderboards** - compare scores with others
- **Custom categories** - movies, sports, companies
- **AI opponents** - single-player mode

### Manufacturing Preparation
- **Document lessons learned** - what worked, what didn't
- **Test component durability** - how many scans before failure?
- **Refine user experience** - eliminate confusion points
- **Calculate real costs** - is $100 manufacturing target realistic?

---

## 🏆 Success Declaration

**You've successfully built a working Know-It-All prototype if:**

✅ **Electronics work reliably** - consistent RFID scanning, clear audio  
✅ **Game flow is intuitive** - players understand without lengthy explanation  
✅ **Educational and fun** - people learn and want to play again  
✅ **Proves the concept** - validates the core RFID ranking mechanic  
✅ **Under budget** - total cost below $100  
✅ **Weekend timeline** - completed in allocated time frame  

**Congratulations! You now have a working proof-of-concept that validates the manufacturing investment and provides a roadmap for the final product.**

---

*Ready to build the future of educational gaming? Start shopping and get building! 🚀*