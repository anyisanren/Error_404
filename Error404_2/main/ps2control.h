#ifndef PS2CONTROL_H
#define PS2CONTROL_H

#include <Arduino.h>
#include <PS2X_lib.h>
#include "movement.h"
#include "initialization.h"
#include "persistent_log.h"

// PS2 pins (Arduino Mega SPI): DAT(MISO)=50 CMD(MOSI)=51 CLK(SCK)=52 ATT(CS)=53
static const uint8_t PS2_DAT = 50;
static const uint8_t PS2_CMD = 51;
static const uint8_t PS2_CLK = 52;
static const uint8_t PS2_ATT = 53;

static PS2X ps2x;
static bool ps2_ok = false;
static unsigned long ps2_last_init_ms = 0;
static const unsigned long PS2_RETRY_INTERVAL_MS = 1000;
static uint8_t ps2_init_mode = 0;
static int8_t ps2_last_logged_error = -127;
static unsigned long ps2_last_error_log_ms = 0;
static const unsigned long PS2_ERROR_LOG_MIN_INTERVAL_MS = 15000;

static inline bool initPS2() {
  // Rotate init modes to improve compatibility with different receivers/power conditions.
  // mode 0: pressures=true,  rumble=false
  // mode 1: pressures=false, rumble=false
  // mode 2: pressures=true,  rumble=true
  // mode 3: pressures=false, rumble=true
  bool pressures = (ps2_init_mode == 0 || ps2_init_mode == 2);
  bool rumble = (ps2_init_mode == 2 || ps2_init_mode == 3);

  ps2_last_init_ms = millis();
  int error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT, pressures, rumble);
  ps2_init_mode = (uint8_t)((ps2_init_mode + 1) & 0x03);

  if (error == 0) {
    ps2_ok = true;
    ps2_last_logged_error = -127;
    logAppend("PS2 init ok");
    return true;
  }
  ps2_ok = false;
  if ((int8_t)error != ps2_last_logged_error ||
      (millis() - ps2_last_error_log_ms) >= PS2_ERROR_LOG_MIN_INTERVAL_MS) {
    logAppendKV("PS2 init err=", error);
    ps2_last_logged_error = (int8_t)error;
    ps2_last_error_log_ms = millis();
  }
  return false;
}

// Call this regularly in loop; maps D-pad to translation (mecanum)
static inline void handlePS2(uint8_t speed = 200) {
  if (!ps2_ok) {
    if (millis() - ps2_last_init_ms >= PS2_RETRY_INTERVAL_MS) {
      initPS2();
    }
    stopAllMotors();
    return;
  }

  ps2x.read_gamepad(false, 0);

  bool up = ps2x.Button(PSB_PAD_UP);
  bool down = ps2x.Button(PSB_PAD_DOWN);
  bool left = ps2x.Button(PSB_PAD_LEFT);
  bool right = ps2x.Button(PSB_PAD_RIGHT);

  // Mapping for mecanum wheels (translation):
  // Forward: all forward
  // Backward: all backward
  // Strafe right: FL forward, FR backward, BL backward, BR forward
  // Strafe left:  FL backward, FR forward,  BL forward,  BR backward

  if (up && !down && !left && !right) {
    // forward
    setMotor(0, MOTOR_FORWARD, speed);
    setMotor(1, MOTOR_FORWARD, speed);
    setMotor(2, MOTOR_FORWARD, speed);
    setMotor(3, MOTOR_FORWARD, speed);
  } else if (down && !up && !left && !right) {
    // backward
    setMotor(0, MOTOR_BACKWARD, speed);
    setMotor(1, MOTOR_BACKWARD, speed);
    setMotor(2, MOTOR_BACKWARD, speed);
    setMotor(3, MOTOR_BACKWARD, speed);
  } else if (left && !up && !down && !right) {
    // strafe left
    setMotor(0, MOTOR_BACKWARD, speed); // FL
    setMotor(1, MOTOR_FORWARD, speed);  // FR
    setMotor(2, MOTOR_FORWARD, speed);  // BL
    setMotor(3, MOTOR_BACKWARD, speed); // BR
  } else if (right && !up && !down && !left) {
    // strafe right
    setMotor(0, MOTOR_FORWARD, speed);  // FL
    setMotor(1, MOTOR_BACKWARD, speed); // FR
    setMotor(2, MOTOR_BACKWARD, speed); // BL
    setMotor(3, MOTOR_FORWARD, speed);  // BR
  } else if (up && left && !down && !right) {
    // forward + left (diagonal) - mix: reduce speed on some wheels for smoother move
    setMotor(0, MOTOR_FORWARD, (uint8_t)(speed * 0.7));
    setMotor(1, MOTOR_FORWARD, (uint8_t)(speed * 0.4));
    setMotor(2, MOTOR_FORWARD, (uint8_t)(speed * 0.4));
    setMotor(3, MOTOR_FORWARD, (uint8_t)(speed * 0.7));
  } else if (up && right && !down && !left) {
    setMotor(0, MOTOR_FORWARD, (uint8_t)(speed * 0.4));
    setMotor(1, MOTOR_FORWARD, (uint8_t)(speed * 0.7));
    setMotor(2, MOTOR_FORWARD, (uint8_t)(speed * 0.7));
    setMotor(3, MOTOR_FORWARD, (uint8_t)(speed * 0.4));
  } else {
    // no directional pad pressed or conflicting inputs -> stop
    stopAllMotors();
  }
}

#endif // PS2CONTROL_H
