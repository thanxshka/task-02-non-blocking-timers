// Task 02: Timers and Non-Blocking Code
// Target: Arduino Uno / Nano (ATmega328P, 16 MHz)
// Task 1: Hardware Timer1 ISR triggers every 1000 ms (1 Hz) -> Toggles LED_SLOW (Pin 13)
// Task 2: millis() scheduler triggers every 250 ms (4 Hz)  -> Toggles LED_FAST (Pin 12)

const int LED_SLOW = 13; // Built-in LED
const int LED_FAST = 12; // Secondary LED

// Volatile flag shared between ISR and main loop
volatile bool timer1Flag = false;

// State variables
bool slowLedState = false;
bool fastLedState = false;

// Non-blocking timing variables for Task 2
unsigned long lastFastToggle = 0;
const unsigned long FAST_INTERVAL_MS = 250;

void setupTimer1() {
  cli(); // Disable global interrupts during setup

  TCCR1A = 0; // Reset Timer1 control registers
  TCCR1B = 0;
  TCNT1  = 0; // Reset counter value

  // Set compare match register for 1 Hz (1000 ms)
  // Formula: (16,000,000 / (prescaler * target_freq)) - 1
  // (16,000,000 / (1024 * 1)) - 1 = 15624
  OCR1A = 15624;

  TCCR1B |= (1 << WGM12);              // Enable CTC (Clear Timer on Compare Match) mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);             // Enable Timer1 compare interrupt

  sei(); // Re-enable global interrupts
}

// Hardware Interrupt Service Routine (kept short and safe)
ISR(TIMER1_COMPA_vect) {
  timer1Flag = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_SLOW, OUTPUT);
  pinMode(LED_FAST, OUTPUT);

  setupTimer1();
  Serial.println("Non-blocking timer system started.");
}

void loop() {
  // --- Task 1: Handled via Hardware Timer1 Interrupt (1000 ms) ---
  if (timer1Flag) {
    timer1Flag = false; // Clear flag immediately
    slowLedState = !slowLedState;
    digitalWrite(LED_SLOW, slowLedState);
    Serial.print("[1000ms - Timer1 ISR] Slow LED: ");
    Serial.println(slowLedState ? "ON" : "OFF");
  }

  // --- Task 2: Handled via millis() non-blocking check (250 ms) ---
  unsigned long currentMillis = millis();
  if (currentMillis - lastFastToggle >= FAST_INTERVAL_MS) {
    lastFastToggle = currentMillis;
    fastLedState = !fastLedState;
    digitalWrite(LED_FAST, fastLedState);
    Serial.print("[250ms  - millis()]   Fast LED: ");
    Serial.println(fastLedState ? "ON" : "OFF");
  }
}
