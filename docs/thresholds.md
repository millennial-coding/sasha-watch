# Alert Thresholds

## Overview
Thresholds are set based on the specific needs of the apartment's inhabitants:
- **Sasha** — dog, home alone during 12-hour overnight shifts
- **Ball python** — requires stable temperature and humidity ranges
- **Tropical plants** — require moderate to high humidity
- **Arizona climate** — swamp cooler apartment, unreliable cooling in summer

## Temperature Thresholds

| Level | Threshold | Reason |
|---|---|---|
| 🟢 Normal | Below 78°F | Comfortable for all inhabitants |
| 🟡 Warning | 78°F | Early alert — conditions degrading |
| 🔴 Critical | 82°F | Arizona legal limit for landlord action |

### Notes
- Arizona law requires landlords with swamp coolers to act within 5 days
  of temperatures exceeding 86°F
- Dogs cool themselves through panting which becomes ineffective above ~85°F
- Early warning at 78°F gives time to act before legal/safety thresholds

## Humidity Thresholds

| Level | Threshold | Reason |
|---|---|---|
| 🟢 Normal | Below 65% | Comfortable for dog and plants |
| 🟡 Warning | 65% | Approaching high end for dog comfort |
| 🔴 Critical | 75% | Risk of mold, respiratory stress |

### Notes
- Ball python optimal range: 60-80% humidity
- Ball python shedding cycle: 80-90% humidity recommended
- Current reading of ~41% is below ideal for ball python (60-80%)
- Low humidity alert (below 40%) to be added in future update
- Tropical plants prefer 60%+ humidity
- High humidity combined with high heat is harder on dogs than dry heat alone

## Future Improvements
- [ ] Add low humidity alert (below 40%) for ball python shed health
- [ ] Add separate enclosure sensor inside snake tank for precise monitoring
- [ ] Investigate humidifier automation via n8n when humidity drops below 50%