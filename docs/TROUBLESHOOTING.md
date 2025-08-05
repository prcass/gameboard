# GameBoard Troubleshooting Guide

## 🚨 Common Hardware Issues

### RFID Not Reading Cards
**Symptoms:** No response when scanning cards, "RFID not responding" in component test

**Most Common Cause:** RFID connected to 5V instead of 3.3V
- **Check:** MFRC522 VCC pin goes to Arduino 3.3V (NOT 5V)
- **Fix:** Move red wire from 5V to 3.3V rail

**Other Causes:**
- **Loose SPI connections** - Check pins 9-13 connections
- **Wrong card frequency** - MFRC522 works with 13.56MHz cards only
- **Damaged RFID module** - Try different cards, check for physical damage

### No Sound from Buzzer
**Symptoms:** Silent operation, no beeps during tests

**Passive vs Active Buzzer:**
- **Passive buzzer** (your type) needs PWM signal - use digitalWrite() HIGH/LOW
- **Active buzzer** would beep continuously with constant power

**Common Fixes:**
- **Check power:** Buzzer VCC to 5V, GND to GND
- **Check signal:** Buzzer signal pin to Arduino Pin 3
- **Test code:** Try simple digitalWrite(BUZZER_PIN, HIGH); delay(1000); digitalWrite(BUZZER_PIN, LOW);

### NeoPixel Ring Not Lighting
**Symptoms:** LEDs remain dark, no colors displayed

**Power Issues:**
- **Check 5V connection** to NeoPixel VCC
- **Ensure common ground** between Arduino and NeoPixel
- **Power capacity** - NeoPixels need significant current

**Data Issues:**
- **Check data wire** from NeoPixel Data In to Arduino Pin 6
- **Library issues** - Ensure Adafruit NeoPixel library installed
- **Wrong pin definition** - Verify NEOPIXEL_PIN = 6 in code

### Arduino Won't Program
**Symptoms:** Upload errors, "port not available"

**USB Connection:**
- **Try different USB cable** - some are charge-only
- **Check Device Manager** (Windows) for COM port
- **Press reset button** on Arduino during upload

**IDE Issues:**
- **Select correct board:** Tools → Board → Arduino Uno
- **Select correct port:** Tools → Port → [your COM port]
- **Close Serial Monitor** during upload

## 🔧 Software Issues

### Library Installation Problems
**Symptoms:** "Library not found" compilation errors

**MFRC522 Library:**
- **Manual install:** Download from https://github.com/miguelbalboa/rfid
- **ZIP install:** Sketch → Include Library → Add .ZIP Library
- **Alternative:** Search for "RFID" instead of "MFRC522"

**NeoPixel Library:**
- **Install via Library Manager:** Search "Adafruit NeoPixel"
- **Restart Arduino IDE** after installation
- **Check version:** Ensure latest version installed

### Compilation Errors
**Symptoms:** Red error messages during code upload

**Common Errors:**
```cpp
// Error: 'MFRC522' was not declared
// Fix: Install MFRC522 library

// Error: 'Adafruit_NeoPixel' was not declared  
// Fix: Install Adafruit NeoPixel library

// Error: 'pixels' was not declared
// Fix: Check #define NEOPIXEL_PIN 6 is correct
```

### Serial Monitor Issues
**Symptoms:** Gibberish text, no output, garbled characters

**Baud Rate Mismatch:**
- **Set Serial Monitor to 9600 baud** (bottom right dropdown)
- **Match code:** Serial.begin(9600) in setup()

**Connection Issues:**
- **USB cable quality** - try different cable
- **Driver issues** - update Arduino drivers
- **Port conflicts** - close other programs using serial port

## 🎮 Game Logic Issues

### Cards Not Recognized
**Symptoms:** "Unknown card" messages, wrong country names

**UID Programming:**
- **Scan each card** with uid_discovery.ino first
- **Update code** with actual UIDs from your stickers
- **Case sensitivity** - UIDs are case-sensitive
- **Format matching** - Ensure UID format matches exactly

### Wrong Challenge Loading
**Symptoms:** GDP challenge loads when scanning population card

**Challenge Card Programming:**
- **Separate UIDs** for each challenge type
- **Update challenge database** with correct UIDs
- **Test each challenge card** individually

### Press-Your-Luck Logic Problems
**Symptoms:** Always correct, always wrong, inconsistent scoring

**Difficulty Tuning:**
```cpp
// In checkIfCorrectForPosition() function
// Adjust these values to make game easier/harder:
long expectedMax = 25000 / position; // Make smaller = harder
long expectedMin = 1000 * position;  // Make larger = harder
```

## ⚡ Power & Performance Issues

### Random Resets/Crashes
**Symptoms:** Arduino restarts during gameplay, code stops responding

**Power Supply Issues:**
- **9V battery getting weak** - replace with fresh battery
- **Power bank output too low** - ensure 2A output capability
- **Voltage drops** - NeoPixels drawing too much current

**Code Issues:**
- **Memory leaks** - restart Arduino if behavior degrades
- **Infinite loops** - check for while() loops without exit conditions

### Short Battery Life
**Symptoms:** 9V battery dies quickly (under 2 hours)

**Power Optimization:**
```cpp
// Reduce NeoPixel brightness to save power
pixels.setPixelColor(i, pixels.Color(50, 50, 50)); // Instead of 255
```

**Hardware Optimization:**
- **Turn off NeoPixels** when not in use: pixels.clear(); pixels.show();
- **Minimize buzzer usage** - shorter beeps save power
- **Use higher capacity batteries** - rechargeable 9V or power bank

### Slow Response Times
**Symptoms:** Delay between card scan and feedback

**RFID Optimization:**
- **Reduce SCAN_DELAY** in code (currently 1000ms)
- **Optimize antenna positioning** - RFID reader flat, cards perpendicular
- **Clean card contacts** - wipe NFC stickers with cloth

## 🛠️ Assembly & Mechanical Issues

### Loose Connections
**Symptoms:** Intermittent failures, works sometimes

**Breadboard Issues:**
- **Push wires fully** into breadboard holes
- **Check for bent pins** on components
- **Use quality jumper wires** - cheap wires break internally

**Permanent Solutions:**
- **Solder connections** for final version
- **Use terminal blocks** for removable but secure connections
- **Cable management** - secure wires to prevent movement

### Component Mounting
**Symptoms:** Components fall off, board unstable

**Mechanical Solutions:**
- **Hot glue** components to foam board base
- **Cable ties** for wire management
- **Enclosure box** for finished prototype

## 📞 Getting Help

### Self-Diagnosis Steps
1. **Upload component_test.ino** - isolate hardware issues
2. **Check Serial Monitor** - look for error messages
3. **Test individual components** - RFID, buzzer, LEDs separately
4. **Verify wiring** against EXACT_COMPONENTS_WIRING.md

### Information to Provide
When asking for help, include:
- **Exact error messages** from Arduino IDE or Serial Monitor
- **Component test results** - which parts pass/fail
- **Photos of wiring** - especially power connections
- **Code modifications** - any changes you made

### Resources
- **Arduino Community Forum:** https://forum.arduino.cc/
- **MFRC522 Library Issues:** https://github.com/miguelbalboa/rfid/issues
- **NeoPixel Troubleshooting:** https://learn.adafruit.com/adafruit-neopixel-uberguide

## ✅ Success Checklist

Before declaring "broken":
- [ ] **Component test passes** - all hardware working
- [ ] **Correct libraries installed** - MFRC522 and NeoPixel
- [ ] **UIDs programmed correctly** - scanned and updated in code
- [ ] **Power supply adequate** - fresh 9V battery or good power bank
- [ ] **Wiring verified** - especially 3.3V to RFID, 5V to others
- [ ] **Code uploaded successfully** - no compilation errors

**Most issues are wiring or power related - double-check connections first!**