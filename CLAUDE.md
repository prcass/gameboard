# GameBoard Oracle - Project Context for Claude Continuation

## 🎯 Project Evolution Summary
**Started as:** Physical RFID prototype board game  
**Evolved into:** Commercial "Oracle" game with truncated pyramid design  
**Current Phase:** Production-ready specifications and manufacturing plan  

## 🏛️ FINAL DESIGN DECISIONS

### The Oracle Device
- **Shape:** Truncated pyramid (pyramid with flat top cut off)
- **Dimensions:** 7" × 7" base × 3" height with 2.5" scanning surface
- **Material:** Vacuum-formed translucent plastic housing
- **Electronics:** Arduino Nano clone, 12-LED NeoPixel ring, basic speaker
- **Manufacturing Cost:** $16 at 2,000 units

### Game Components
- **30 Round NFC Tokens:** Countries ($11.50 for 30 at retail)
- **30 CR80 NFC Cards:** Challenges (from $45.99 100-pack)
- **Technology:** 13.56MHz RFID (NTAG215/MIFARE Classic)
- **Total Game:** 60 NFC-enabled components + Oracle device

### Business Model
- **Base Game:** $69.99 CAD retail (profitable at 2,000+ units)
- **Expansions:** $24.99-29.99 CAD (new categories: Movies, Sports, etc.)
- **Manufacturing:** Vietnam assembly, direct component sourcing

## 🔧 Working Prototype Status

### Hardware COMPLETE ✅
- Arduino Uno R3 SMD with breadboard circuit
- MFRC522 RFID reader (3.3V power - critical!)
- 12-LED NeoPixel ring, passive buzzer
- All components tested and working

### Software COMPLETE ✅
**Key Files in GitHub:**
- `gameboard_real_logic.ino` - Complete game with proper ranking validation
- `gameboard_optimized.ino` - RFID reliability improvements
- Fixed GDP per capita calculation overflow issues
- Real-time country data display in Serial Monitor

### Physical Components STATUS
- **10 country tokens programmed** with actual UIDs
- **4 challenge cards programmed** with actual UIDs
- **All UIDs recorded** in scanned_uids.txt
- **One token (poker chip) intermittently fails** due to material interference

## 🏷️ Actual Scanned UIDs (WORKING)
```
Countries:
USA: "1D 5B 00 76 0D 10 80"
China: "1D 0E 0E 76 0D 10 80" 
Japan: "1D EC 10 76 0D 10 80"
Germany: "1D 05 1E 76 0D 10 80"
India: "1D 0D 21 76 0D 10 80"
UK: "1D 43 2E 76 0D 10 80"
France: "1D 23 31 76 0D 10 80"
Italy: "1D 30 3D 76 0D 10 80"
Brazil: "1D 39 40 76 0D 10 80"
Canada: "1D 42 4D 76 0D 10 80"

Challenges:
GDP: "1D C2 30 76 0D 10 80"
Population: "1D E1 4C 76 0D 10 80"  
Area: "1D D8 3F 76 0D 10 80"
GDP per Capita: "1D D1 3C 76 0D 10 80"
```

## 💰 Manufacturing Economics (FINALIZED)

### At 2,000 Units Production
- **Total Manufacturing:** $38 (all optimizations applied)
- **Shipping to Canada:** $8
- **Total Landed Cost:** $46
- **Profit Margins:** $1-10 per unit across all retail channels

### Cost Optimizations Applied
1. Arduino Nano clone (-$3)
2. 12-LED ring instead of 20 (-$2)  
3. Vietnam assembly (-$3)
4. Direct component sourcing (-$2)
5. Pyramid housing 7×7×3 (-$1.50)

## 🎮 Gameplay Mechanics (PROVEN)

### Press Your Luck Flow
1. **Challenge Setup:** Scan challenge card → Oracle announces category
2. **Token Scanning:** Players take turns scanning country tokens
3. **Immediate Feedback:** Each scan = CORRECT (continue?) or WRONG (game over!)
4. **Banking Decision:** Scan challenge card again to bank points
5. **Social Tension:** Players watch each scan, decide pass/continue

### Technical Validation
- **Real ranking logic implemented** - validates actual country data
- **Proper GDP per capita calculations** - fixed overflow issues  
- **Immediate audio/visual feedback** - LEDs + buzzer patterns
- **No app required** - completely standalone device

## 🌍 Market Research COMPLETED

### Competitive Analysis
- **No direct competitors** with pyramid Oracle + NFC tokens
- **Electronic board games:** Mostly $80-120 retail
- **Similar products:** Beasts of Balance, Dropmix (both failed/limited success)
- **Unique position:** Physical tokens + no app dependency

### Publishing Assessment
- **8-10% royalty** would be standard industry rate
- **Pyramid Oracle design** is genuinely unique and memorable
- **Manufacturing costs proven** viable for retail channels
- **Expansion model** provides recurring revenue potential

## 📁 Repository Structure
```
/gameboard/
├── arduino/
│   ├── gameboard_real_logic.ino     ← CURRENT WORKING VERSION
│   ├── gameboard_optimized.ino      ← RFID reliability fixes  
│   └── component_test.ino           ← Hardware validation
├── PROJECT_SUMMARY.md               ← Complete specifications
├── CLAUDE.md                        ← This context file
└── scanned_uids.txt                ← All working UIDs
```

## 🚀 NEXT PHASE PRIORITIES

### Immediate (Next Session)
1. **Build pyramid prototype** - 3D print or cardboard mockup
2. **Source production NFC components** - Order the $11.50 + $45.99 packs
3. **Test complete user experience** with pyramid Oracle
4. **Create demo video** for potential publishers/investors

### Short Term (3-6 months)
1. **Publisher meetings** with working pyramid prototype
2. **Manufacturing partner identification** (Vietnam assembly)
3. **Tooling costs** for pyramid vacuum forming
4. **Regulatory compliance** (CE, FCC for electronics)

## ⚠️ Critical Technical Notes
- **RFID power:** MUST use 3.3V (not 5V) for MFRC522
- **Intermittent scanning:** Reset Arduino if RFID locks up
- **Component quality:** Cheap NFC stickers have 5-10% failure rate
- **GDP calculation:** Fixed integer overflow in per-capita calculations

## 🎯 Success Criteria Defined
- **Break-even:** 1,500 units sold
- **Profitable growth:** 2,000+ units with expansion sales
- **Technical:** >95% scan success rate, <2% defect rate
- **Market:** 4+ star customer reviews, word-of-mouth growth

**Status:** Ready for pyramid prototype construction and publisher demos  
**Investment Required:** $46K for 2,000 unit production run  
**Revenue Potential:** $180K+ in Year 1 at conservative projections  

*Last Updated: August 7, 2025*