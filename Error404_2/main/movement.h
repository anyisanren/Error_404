#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <Arduino.h>
#include "initialization.h"

// Simple per-wheel movement helpers. Implemented as static inline to allow inclusion in a single-file Arduino sketch.

// Direction constants for setMotor convenience
enum MotorDir { MOTOR_STOP = 0, MOTOR_FORWARD = 1, MOTOR_BACKWARD = 2 };

// FRONT LEFT
static inline void frontLeftForward(uint8_t speed) {
  digitalWrite(FL_IN1, HIGH);
  digitalWrite(FL_IN2, LOW);
  analogWrite(FL_EN, speed);
}
static inline void frontLeftBackward(uint8_t speed) {
  digitalWrite(FL_IN1, LOW);
  digitalWrite(FL_IN2, HIGH);
  analogWrite(FL_EN, speed);
}
static inline void frontLeftStop() {
  digitalWrite(FL_IN1, LOW);
  digitalWrite(FL_IN2, LOW);
  analogWrite(FL_EN, 0);
}

// FRONT RIGHT
static inline void frontRightForward(uint8_t speed) {
  digitalWrite(FR_IN1, HIGH);
  digitalWrite(FR_IN2, LOW);
  analogWrite(FR_EN, speed);
}
static inline void frontRightBackward(uint8_t speed) {
  digitalWrite(FR_IN1, LOW);
  digitalWrite(FR_IN2, HIGH);
  analogWrite(FR_EN, speed);
}
static inline void frontRightStop() {
  digitalWrite(FR_IN1, LOW);
  digitalWrite(FR_IN2, LOW);
  analogWrite(FR_EN, 0);
}

// BACK LEFT
static inline void backLeftForward(uint8_t speed) {
  digitalWrite(BL_IN1, LOW);
  digitalWrite(BL_IN2, HIGH);
  analogWrite(BL_EN, speed);
}
static inline void backLeftBackward(uint8_t speed) {
  digitalWrite(BL_IN1, HIGH);
  digitalWrite(BL_IN2, LOW);
  analogWrite(BL_EN, speed);
}
static inline void backLeftStop() {
  digitalWrite(BL_IN1, LOW);
  digitalWrite(BL_IN2, LOW);
  analogWrite(BL_EN, 0);
}

// BACK RIGHT
static inline void backRightForward(uint8_t speed) {
  digitalWrite(BR_IN1, LOW);
  digitalWrite(BR_IN2, HIGH);
  analogWrite(BR_EN, speed);
}
static inline void backRightBackward(uint8_t speed) {
  digitalWrite(BR_IN1, HIGH);
  digitalWrite(BR_IN2, LOW);
  analogWrite(BR_EN, speed);
}
static inline void backRightStop() {
  digitalWrite(BR_IN1, LOW);
  digitalWrite(BR_IN2, LOW);
  analogWrite(BR_EN, 0);
}

// Convenience functions
static inline void stopAllMotors() {
  frontLeftStop();
  frontRightStop();
  backLeftStop();
  backRightStop();
}

// index: 0=FL,1=FR,2=BL,3=BR
static inline void setMotor(uint8_t index, MotorDir dir, uint8_t speed) {
  switch (index) {
    case 0:
      if (dir == MOTOR_FORWARD) frontLeftForward(speed);
      else if (dir == MOTOR_BACKWARD) frontLeftBackward(speed);
      else frontLeftStop();
      break;
    case 1:
      if (dir == MOTOR_FORWARD) frontRightForward(speed);
      else if (dir == MOTOR_BACKWARD) frontRightBackward(speed);
      else frontRightStop();
      break;
    case 2:
      if (dir == MOTOR_FORWARD) backLeftForward(speed);
      else if (dir == MOTOR_BACKWARD) backLeftBackward(speed);
      else backLeftStop();
      break;
    case 3:
      if (dir == MOTOR_FORWARD) backRightForward(speed);
      else if (dir == MOTOR_BACKWARD) backRightBackward(speed);
      else backRightStop();
      break;
    default:
      // invalid index -> stop
      stopAllMotors();
      break;
  }
}

#endif // MOVEMENT_H
