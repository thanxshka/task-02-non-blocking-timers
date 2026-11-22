# Task 02: Timers and Non-Blocking Code

A non-blocking embedded loop for Arduino (ATmega328P) that runs two tasks at different rates without using `delay()`.

## What It Does
- **Task 1 (1000 ms / 1 Hz):** Uses a **Hardware Timer1 Compare Match Interrupt (CTC mode)** to toggle the built-in LED on Pin 13.
- **Task 2 (250 ms / 4 Hz):** Uses a non-blocking `millis()` timestamp check to toggle a secondary LED on Pin 12.
- Both tasks log their state changes over Serial at `115200` baud without blocking each other.

## Why Non-Blocking & Hardware Timers?
- **No `delay()`:** `delay()` halts the CPU, preventing the microcontroller from reading sensors, handling communication, or running other tasks.
- **Accurate Timing:** Using Timer1 in CTC mode guarantees a hardware-accurate 1-second tick independent of how long `loop()` takes to execute.
- **Safe ISR Design:** The Interrupt Service Routine (`ISR(TIMER1_COMPA_vect)`) only sets a `volatile bool` flag. Actual I/O (`digitalWrite` and `Serial.println`) is deferred to `loop()` to avoid blocking other interrupts or corrupting serial output.
- **Rollover-Safe `millis()`:** Using subtraction (`currentMillis - lastFastToggle >= FAST_INTERVAL_MS`) prevents bugs when the `millis()` counter overflows after ~49.7 days.

## How to Run
### Option A: Wokwi Simulator (Fastest)
1. Go to [wokwi.com](https://wokwi.com) and start a new **Arduino Uno** project.
2. Paste the contents of `non_blocking_timer.ino` into the editor.
3. (Optional) Connect an LED + 220Ω resistor to Pin 12 and GND (Pin 13 uses the onboard LED).
4. Click **Start Simulation** and watch the Serial Monitor and LEDs.

### Option B: Arduino IDE
1. Open `non_blocking_timer.ino` in the Arduino IDE.
2. Select **Board: Arduino Uno** and your COM port.
3. Click **Upload** and open the Serial Monitor at **115200 baud**.
