#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include <Arduino.h>

// Pin mapping - modify to match your wiring
// EN pins should be PWM-capable on Mega (e.g., 2,3,4,5,44,45,46)
// Front L298N module: IN1,IN2,IN3,IN4 = 25,24,23,22 ; ENA=4 ENB=5
static const uint8_t FL_IN1 = 25;
static const uint8_t FL_IN2 = 24;
static const uint8_t FL_EN  = 4;   // Front-Left PWM (ENA)

static const uint8_t FR_IN1 = 23;
static const uint8_t FR_IN2 = 22;
static const uint8_t FR_EN  = 5;   // Front-Right PWM (ENB)

// Back L298N module: IN1,IN2,IN3,IN4 = 31,30,29,28 ; ENA=2 ENB=3
static const uint8_t BL_IN1 = 29;
static const uint8_t BL_IN2 = 28;
static const uint8_t BL_EN  = 2;   // Back-Left PWM (ENA)

static const uint8_t BR_IN1 = 27;
static const uint8_t BR_IN2 = 26;
static const uint8_t BR_EN  = 3;   // Back-Right PWM (ENB)

// Initialize pin directions and ensure motors are stopped (does not call stopAllMotors to avoid header cycle)
static inline void initPins() {
  // direction pins
  pinMode(FL_IN1, OUTPUT); pinMode(FL_IN2, OUTPUT); pinMode(FL_EN, OUTPUT);
  pinMode(FR_IN1, OUTPUT); pinMode(FR_IN2, OUTPUT); pinMode(FR_EN, OUTPUT);
  pinMode(BL_IN1, OUTPUT); pinMode(BL_IN2, OUTPUT); pinMode(BL_EN, OUTPUT);
  pinMode(BR_IN1, OUTPUT); pinMode(BR_IN2, OUTPUT); pinMode(BR_EN, OUTPUT);

  // default to stopped
  digitalWrite(FL_IN1, LOW); digitalWrite(FL_IN2, LOW); analogWrite(FL_EN, 0);
  digitalWrite(FR_IN1, LOW); digitalWrite(FR_IN2, LOW); analogWrite(FR_EN, 0);
  digitalWrite(BL_IN1, LOW); digitalWrite(BL_IN2, LOW); analogWrite(BL_EN, 0);
  digitalWrite(BR_IN1, LOW); digitalWrite(BR_IN2, LOW); analogWrite(BR_EN, 0);
}

#endif // INITIALIZATION_H
