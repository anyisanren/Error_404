#include "initialization.h"
#include "movement.h"
#include "ps2control.h"
#include "persistent_log.h"

void setup() {
  Serial.begin(115200);
  logEnsureInit();
  logAppend("BOOT");
  initPins();
  stopAllMotors();
  delay(1500); // wait PS2 Bluetooth receiver to power up when using external supply
  Serial.println("PS2 → Bluetooth control test");
  Serial.println("Type 'L' to dump EEPROM logs, 'C' to clear logs.");

  bool ok = initPS2();
  if (ok) Serial.println("PS2 controller initialized");
  else Serial.println("PS2 init failed - check wiring and library");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = (char)Serial.read();
    if (cmd == 'L' || cmd == 'l') {
      logDumpToSerial(Serial);
    } else if (cmd == 'C' || cmd == 'c') {
      logClear();
      Serial.println("[EEPROM LOG] cleared");
      logAppend("LOG CLEARED");
    }
  }

  // Read PS2 and control movement. Adjust speed if needed.
  handlePS2(200);
  delay(50);
}
