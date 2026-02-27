# GasGuard

A compact, wall-mountable safety system that detects flammable gas leaks in
enclosed spaces at an early stage and informs the user with **visual** and
**auditory** warnings.

---

## Features

| Feature | Description |
|---|---|
| Early detection | Two-level alert system (WARNING + DANGER) before concentrations become critical |
| Visual indicators | Green / Yellow / Red LED for at-a-glance status |
| Auditory alarm | Distinct buzzer patterns for WARNING and DANGER states |
| Non-blocking firmware | Sensor sampling and buzzer patterns run concurrently without `delay()` |
| Serial debugging | Readings and state changes printed to the Serial Monitor at 9600 baud |

---

## Hardware

| Component | Quantity | Notes |
|---|---|---|
| Arduino Uno or Nano | 1 | Any 5 V AVR board works |
| MQ-2 gas sensor module | 1 | Detects LPG, propane, methane, hydrogen |
| Green LED | 1 | SAFE indicator |
| Yellow LED | 1 | WARNING indicator |
| Red LED | 1 | DANGER indicator |
| Active buzzer | 1 | 5 V, any small through-hole type |
| 220 Ω resistor | 3 | Current-limiting resistors for LEDs |
| Breadboard + jumper wires | — | For prototyping |

---

## Wiring / Pin Assignments

```
Arduino Pin   Component
───────────   ─────────────────────────────────────────────────
A0            MQ-2 AOUT (analog output)
5 V           MQ-2 VCC
GND           MQ-2 GND
D2            Green  LED anode  → 220 Ω → GND
D3            Yellow LED anode  → 220 Ω → GND
D4            Red    LED anode  → 220 Ω → GND
D5            Buzzer (+) pin    → GND (– pin)
```

---

## Detection Thresholds

The raw ADC value from the MQ-2 sensor (0–1023) determines the system state:

| State   | ADC Range        | Green LED | Yellow LED | Red LED | Buzzer |
|---------|-----------------|-----------|------------|---------|--------|
| SAFE    | < 300           | ✅ ON     | off        | off     | silent |
| WARNING | 300 – 599       | off       | ✅ ON      | off     | single short beep every 2 s |
| DANGER  | ≥ 600           | off       | off        | ✅ ON   | rapid continuous beeping |

> **Calibration:** The thresholds `WARNING_THRESHOLD` (300) and
> `DANGER_THRESHOLD` (600) are defined as constants at the top of
> `GasGuard.ino`. Adjust them to suit your environment after calibrating
> the MQ-2 sensor according to its datasheet.

---

## Startup Behaviour

On power-on the MQ-2 heater requires a **20-second warm-up** period before
readings are reliable. During this time the green LED blinks at 1 Hz to
indicate that the system is initialising. Normal monitoring begins
automatically once the warm-up is complete.

---

## Building & Uploading

1. Open `GasGuard/GasGuard.ino` in the [Arduino IDE](https://www.arduino.cc/en/software).
2. Select your board under **Tools → Board** (e.g., *Arduino Uno*).
3. Select the correct COM port under **Tools → Port**.
4. Click **Upload**.
5. Open **Tools → Serial Monitor** at **9600 baud** to watch live readings.

---

## Repository Structure

```
GasGuard/
├── GasGuard/
│   └── GasGuard.ino   ← Main Arduino sketch
└── README.md
```

---

## License

This project is released under the [MIT License](LICENSE).
