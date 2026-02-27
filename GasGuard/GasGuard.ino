/*
 * GasGuard - Flammable Gas Leak Detection and Early Warning System
 *
 * A compact, wall-mountable safety system that detects flammable gas leaks
 * in enclosed spaces and alerts the user with visual and auditory warnings.
 *
 * Hardware:
 *   - Arduino Uno / Nano
 *   - MQ-2 gas sensor (detects LPG, propane, methane, hydrogen)
 *   - Green LED  → SAFE status indicator
 *   - Yellow LED → WARNING status indicator (early detection)
 *   - Red LED    → DANGER status indicator
 *   - Buzzer     → Auditory alarm
 *
 * Pin assignments (see README for wiring diagram):
 *   A0  - MQ-2 analog output
 *   D2  - Green LED  (SAFE)
 *   D3  - Yellow LED (WARNING)
 *   D4  - Red LED    (DANGER)
 *   D5  - Buzzer
 *
 * Detection thresholds (raw ADC value, 0–1023):
 *   SAFE    : sensor value < WARNING_THRESHOLD
 *   WARNING : WARNING_THRESHOLD ≤ sensor value < DANGER_THRESHOLD
 *   DANGER  : sensor value ≥ DANGER_THRESHOLD
 *
 * Auditory patterns:
 *   WARNING : single short beep every 2 seconds
 *   DANGER  : rapid continuous beeping
 */

// ── Pin definitions ──────────────────────────────────────────────────────────
const int GAS_SENSOR_PIN  = A0;
const int LED_GREEN_PIN   = 2;
const int LED_YELLOW_PIN  = 3;
const int LED_RED_PIN     = 4;
const int BUZZER_PIN      = 5;

// ── Detection thresholds ─────────────────────────────────────────────────────
const int WARNING_THRESHOLD = 300;  // ADC counts – early warning level
const int DANGER_THRESHOLD  = 600;  // ADC counts – danger / alarm level

// ── Sensor warm-up ───────────────────────────────────────────────────────────
const unsigned long WARMUP_MS = 20000UL;  // 20 s warm-up for MQ-2 heater

// ── Timing constants ─────────────────────────────────────────────────────────
const unsigned int SAMPLE_INTERVAL_MS   = 500;   // sensor polling interval
const unsigned int WARNING_BEEP_ON_MS   = 100;   // short warning beep duration
const unsigned int WARNING_BEEP_OFF_MS  = 1900;  // silence between warning beeps
const unsigned int DANGER_BEEP_ON_MS    = 100;   // rapid danger beep on time
const unsigned int DANGER_BEEP_OFF_MS   = 100;   // rapid danger beep off time

// ── State enum ───────────────────────────────────────────────────────────────
enum GasState { STATE_SAFE, STATE_WARNING, STATE_DANGER };

// ── Global variables ─────────────────────────────────────────────────────────
GasState currentState     = STATE_SAFE;
bool     buzzerOn         = false;
unsigned long lastSampleTime  = 0;
unsigned long lastBuzzerToggle = 0;

// ─────────────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(9600);

    pinMode(LED_GREEN_PIN,  OUTPUT);
    pinMode(LED_YELLOW_PIN, OUTPUT);
    pinMode(LED_RED_PIN,    OUTPUT);
    pinMode(BUZZER_PIN,     OUTPUT);

    // All LEDs off, buzzer off
    setAllLeds(LOW);
    digitalWrite(BUZZER_PIN, LOW);

    warmUp();
}

// ─────────────────────────────────────────────────────────────────────────────
void loop() {
    unsigned long now = millis();

    // ── Sample sensor at configured interval ─────────────────────────────────
    if (now - lastSampleTime >= SAMPLE_INTERVAL_MS) {
        lastSampleTime = now;

        int sensorValue = analogRead(GAS_SENSOR_PIN);
        GasState newState = classifyReading(sensorValue);

        if (newState != currentState) {
            currentState = newState;
            lastBuzzerToggle = now;
            buzzerOn = false;
            // Silence buzzer on state change; pattern restarts below
            digitalWrite(BUZZER_PIN, LOW);
        }

        updateLeds();

        Serial.print(F("Gas sensor: "));
        Serial.print(sensorValue);
        Serial.print(F("  State: "));
        Serial.println(stateLabel(currentState));
    }

    // ── Drive buzzer pattern ──────────────────────────────────────────────────
    updateBuzzer(now);
}

// ─────────────────────────────────────────────────────────────────────────────
// Sensor warm-up: blink green LED while waiting, then begin normal operation.
void warmUp() {
    Serial.println(F("GasGuard starting – warming up sensor…"));
    unsigned long start = millis();
    bool ledState = LOW;

    while (millis() - start < WARMUP_MS) {
        ledState = !ledState;
        digitalWrite(LED_GREEN_PIN, ledState);
        delay(500);
    }

    digitalWrite(LED_GREEN_PIN, HIGH);
    Serial.println(F("Warm-up complete. Monitoring started."));
}

// ─────────────────────────────────────────────────────────────────────────────
// Map raw ADC reading to a gas-state enum value.
GasState classifyReading(int value) {
    if (value >= DANGER_THRESHOLD)  return STATE_DANGER;
    if (value >= WARNING_THRESHOLD) return STATE_WARNING;
    return STATE_SAFE;
}

// ─────────────────────────────────────────────────────────────────────────────
// Set all three LEDs to the same level.
void setAllLeds(int level) {
    digitalWrite(LED_GREEN_PIN,  level);
    digitalWrite(LED_YELLOW_PIN, level);
    digitalWrite(LED_RED_PIN,    level);
}

// ─────────────────────────────────────────────────────────────────────────────
// Turn on only the LED matching the current state.
void updateLeds() {
    setAllLeds(LOW);
    switch (currentState) {
        case STATE_SAFE:    digitalWrite(LED_GREEN_PIN,  HIGH); break;
        case STATE_WARNING: digitalWrite(LED_YELLOW_PIN, HIGH); break;
        case STATE_DANGER:  digitalWrite(LED_RED_PIN,    HIGH); break;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Non-blocking buzzer pattern driver.
void updateBuzzer(unsigned long now) {
    if (currentState == STATE_SAFE) {
        digitalWrite(BUZZER_PIN, LOW);
        buzzerOn = false;
        return;
    }

    unsigned int onTime  = (currentState == STATE_DANGER) ? DANGER_BEEP_ON_MS  : WARNING_BEEP_ON_MS;
    unsigned int offTime = (currentState == STATE_DANGER) ? DANGER_BEEP_OFF_MS : WARNING_BEEP_OFF_MS;
    unsigned int elapsed = (unsigned int)(now - lastBuzzerToggle);

    if (buzzerOn && elapsed >= onTime) {
        buzzerOn = false;
        digitalWrite(BUZZER_PIN, LOW);
        lastBuzzerToggle = now;
    } else if (!buzzerOn && elapsed >= offTime) {
        buzzerOn = true;
        digitalWrite(BUZZER_PIN, HIGH);
        lastBuzzerToggle = now;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Return a human-readable label for a gas state.
const __FlashStringHelper* stateLabel(GasState state) {
    switch (state) {
        case STATE_WARNING: return F("WARNING");
        case STATE_DANGER:  return F("DANGER");
        default:            return F("SAFE");
    }
}
