# Know-It-All Prototype - Assembly Checklist

## 📦 Component Verification

Before starting, verify all components are present:

### Electronics
- [ ] **Arduino Uno R3** - Official board with USB cable
- [ ] **IZOKEE MFRC522 RFID Kit** - Module + test cards + key fob
- [ ] **12-LED NeoPixel Ring** - WS2812B compatible 
- [ ] **Elegoo Wire Kit** - Male-to-male jumper wires
- [ ] **Elegoo Breadboard** - Half-size prototyping board
- [ ] **Passive Buzzer Module** - PWM-driven audio feedback
- [ ] **DKARDU 9V Battery Holder** - With DC jack connector
- [ ] **Fresh 9V Battery** - Alkaline preferred

### Game Materials  
- [ ] **PEUTIER NFC Stickers** - 13.56MHz compatible
- [ ] **Token bases** - Wooden discs, poker chips, or cardboard circles
- [ ] **Printed labels** - Country names and data
- [ ] **Challenge cards** - Printed on cardstock

### Tools Needed
- [ ] **Computer** - For Arduino IDE and code upload
- [ ] **Scissors** - For cutting labels and cards  
- [ ] **Craft knife** - For precise cuts (optional)
- [ ] **Ruler** - For measuring positions

---

## ⚡ Power & Safety Check

### Before Connecting Power
- [ ] **Voltage check**: MFRC522 connected to 3.3V (NOT 5V)
- [ ] **Ground connections**: All GND wires secure
- [ ] **No short circuits**: No stray wires touching
- [ ] **Components seated**: RFID module fully inserted in breadboard

### Power-On Test
- [ ] **9V battery installed** in holder
- [ ] **DC jack connected** to Arduino power port
- [ ] **Arduino power LED** lights up green
- [ ] **No smoke or heat** from any components
- [ ] **NeoPixel ring** shows brief startup flash (if code loaded)

---

## 🔧 Assembly Sequence (60 minutes)

### Phase 1: Breadboard Setup (15 minutes)
- [ ] **Power rails connected** - 3.3V and 5V from Arduino
- [ ] **Ground rails bridged** - Both sides connected
- [ ] **MFRC522 positioned** - Straddles center divide in rows 5-8
- [ ] **Power wiring verified** - 3.3V to RFID, 5V to NeoPixel/Buzzer

### Phase 2: RFID Wiring (15 minutes)
- [ ] **VCC → 3.3V** (Red wire) - CRITICAL: Not 5V!
- [ ] **GND → GND** (Black wire)
- [ ] **RST → Pin 9** (Yellow wire)
- [ ] **MISO → Pin 12** (Blue wire)
- [ ] **MOSI → Pin 11** (Green wire)
- [ ] **SCK → Pin 13** (White wire)
- [ ] **SDA → Pin 10** (Orange wire)

### Phase 3: NeoPixel & Buzzer (15 minutes)
- [ ] **NeoPixel VCC → 5V** (Red wire)
- [ ] **NeoPixel GND → GND** (Black wire)
- [ ] **NeoPixel Data → Pin 6** (Green wire)
- [ ] **Buzzer VCC → 5V** (Red wire)
- [ ] **Buzzer GND → GND** (Black wire)
- [ ] **Buzzer Signal → Pin 3** (Yellow wire)

### Phase 4: Code Upload & Test (15 minutes)
- [ ] **Arduino IDE installed** on computer
- [ ] **Adafruit NeoPixel library** installed
- [ ] **RFID library** installed (MFRC522)
- [ ] **Test code uploaded** successfully
- [ ] **Serial Monitor working** (9600 baud)
- [ ] **All components respond** to test sequence

---

## 🧪 Component Testing Protocol

### Test 1: Power & LEDs (2 minutes)
**Expected:** Power LED on Arduino lights up, NeoPixel ring shows red sweep
```
Result: PASS ☐  FAIL ☐
Notes: _________________________________
```

### Test 2: Buzzer Audio (1 minute)  
**Expected:** Short beep sound from buzzer module
```
Result: PASS ☐  FAIL ☐
Notes: _________________________________
```

### Test 3: RFID Recognition (2 minutes)
**Expected:** Scan test card → Serial Monitor shows "Card detected!" + green flash + beep
```
Result: PASS ☐  FAIL ☐
Notes: _________________________________
```

### Test 4: Multiple Cards (3 minutes)
**Expected:** Each different card/sticker produces same response
```
Card 1: PASS ☐  FAIL ☐
Card 2: PASS ☐  FAIL ☐ 
Card 3: PASS ☐  FAIL ☐
Notes: _________________________________
```

---

## 🎮 Game Setup Phase

### NFC Sticker Programming (30 minutes)
- [ ] **Scan each sticker** with test code to get UID
- [ ] **Record UIDs** in programming log
- [ ] **Apply stickers to token bases** - Center and secure
- [ ] **Update main game code** with actual UIDs
- [ ] **Upload complete game code** to Arduino

### Token Creation (20 minutes)
- [ ] **Print country labels** on adhesive paper
- [ ] **Cut labels to size** - 1" circles or appropriate shape
- [ ] **Apply labels to tokens** - Over NFC stickers
- [ ] **Test each token** - Should announce country name (debug mode)

### Challenge Cards (15 minutes)
- [ ] **Print challenge cards** on cardstock 
- [ ] **Apply NFC stickers** to card backs
- [ ] **Record challenge UIDs** and update code
- [ ] **Test each challenge** - Should load appropriate challenge type

---

## 🐛 Troubleshooting Quick Fixes

### RFID Not Reading
**Symptoms:** No response when scanning cards
- [ ] Check 3.3V connection (not 5V)
- [ ] Verify SPI pin connections (especially SDA to pin 10)
- [ ] Try different cards/stickers
- [ ] Check for loose breadboard connections

### No Sound from Buzzer
**Symptoms:** Silent operation, no beeps
- [ ] Verify buzzer type (active vs passive)
- [ ] Check 5V power connection  
- [ ] Test with simple digitalWrite() HIGH/LOW
- [ ] Try different buzzer if available

### NeoPixel Ring Not Lighting
**Symptoms:** LEDs remain dark
- [ ] Check 5V power connection
- [ ] Verify data connection to Pin 6
- [ ] Test with simple pixel.setPixelColor() commands
- [ ] Check for proper library installation

### Arduino Won't Program
**Symptoms:** Upload errors, connection failed
- [ ] Check USB cable (try different cable)
- [ ] Verify correct board selection in Arduino IDE
- [ ] Press reset button before upload
- [ ] Disconnect power during programming

---

## ✅ Final Readiness Check

### Before First Game Session
- [ ] **All components tested** and working
- [ ] **Game code uploaded** with correct UIDs
- [ ] **Tokens respond correctly** when scanned
- [ ] **Challenge cards load** appropriate challenges
- [ ] **Battery provides** 4+ hours runtime
- [ ] **Game flow understood** - scan challenge → scan tokens → get feedback

### Success Criteria Met
- [ ] **RFID scanning feels magical** - instant response
- [ ] **Audio feedback is clear** and helpful
- [ ] **Visual feedback is dramatic** - green success, red failure
- [ ] **Press-your-luck tension** builds with each scan
- [ ] **Family/friends want to play again** after demo

---

## 🎯 Ready to Play!

**Congratulations!** You've built a working RFID prototype that demonstrates:

✅ **Immediate feedback** - Each scan gives instant audio/visual response  
✅ **Press-your-luck mechanics** - Risk/reward with each additional scan  
✅ **Educational content** - Real country data with ranking challenges  
✅ **Authentic feel** - Just like scanning with a real RFID scanner  

**Game Experience:**
1. **Scan challenge card** → Blue pulse + beep (challenge loaded)
2. **Scan country token** → Yellow spinning + rapid beeps (processing)
3. **Wait for result** → Green flash + happy beep (correct) OR Red flash + sad beep (wrong)
4. **Press your luck** → Continue scanning or bank points by scanning challenge card again

**Next Steps:**
- Test with family and friends
- Gather feedback on user experience  
- Document what works and what needs improvement
- Use learnings to refine the manufacturing design

**You've successfully proven the concept! 🎉**