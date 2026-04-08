#include "initialization.h"
#include "movement.h"

const char* wheelNames[4] = {"Front-Left", "Front-Right", "Back-Left", "Back-Right"};

void setup() {
  Serial.begin(115200);
  initPins();
  // make sure all motors are stopped on start
  stopAllMotors();
  Serial.println("Motor test start");
}

void loop() {
  // Individual wheel test sequence: each wheel forward -> stop -> backward -> stop
  const uint8_t testSpeed = 180; // 0-255
  const unsigned long moveMs = 1500;
  const unsigned long pauseMs = 500;
  const unsigned long betweenRoundsMs = 1000;

  for (uint8_t i = 0; i < 4; ++i) {
    Serial.print(wheelNames[i]);
    Serial.println(" - forward");
    setMotor(i, MOTOR_FORWARD, testSpeed);
    delay(moveMs);
    stopAllMotors();
    delay(pauseMs);

    Serial.print(wheelNames[i]);
    Serial.println(" - backward");
    setMotor(i, MOTOR_BACKWARD, testSpeed);
    delay(moveMs);
    stopAllMotors();
    delay(pauseMs);
  }

  Serial.println("Round complete, pausing...");
  delay(betweenRoundsMs);
}
