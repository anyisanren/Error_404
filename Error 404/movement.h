#ifndef MOVEMENT_H
#define MOVEMENT_H

// Serial port configuration for motor controllers
// Serial1: TX=18, RX=19 - Front board (front wheels)
// Serial3: TX=14, RX=15 - Rear board (rear wheels)
#define FRONT_BOARD_SERIAL Serial1
#define REAR_BOARD_SERIAL Serial3
#define BOARD_BAUD 9600

// Wheel definitions
#define FRONT_LEFT 1
#define FRONT_RIGHT 2
#define REAR_LEFT 3
#define REAR_RIGHT 4

// Movement directions
#define MOVE_FORWARD 1
#define MOVE_BACKWARD -1
#define MOVE_STOP 0

// Speed range
#define MAX_SPEED 255
#define MIN_SPEED 0

// Function declarations
void initMovement();
void moveWheel(uint8_t wheel, int8_t speed);
void moveFrontLeft(int8_t speed);
void moveFrontRight(int8_t speed);
void moveRearLeft(int8_t speed);
void moveRearRight(int8_t speed);
void stopAllWheels();

// Initialize movement control
void initMovement() {
  FRONT_BOARD_SERIAL.begin(BOARD_BAUD);
  REAR_BOARD_SERIAL.begin(BOARD_BAUD);
  delay(300);
  
  Serial.println("Movement control boards initialized");
  Serial.println("  Front board: Serial1 (Pin 18,19)");
  Serial.println("  Rear board: Serial3 (Pin 14,15)");
  
  stopAllWheels();
}

// Control single wheel
// speed: -255 (max reverse) to 0 (stop) to 255 (max forward)
void moveWheel(uint8_t wheel, int8_t speed) {
  if (speed > MAX_SPEED) speed = MAX_SPEED;
  if (speed < -MAX_SPEED) speed = -MAX_SPEED;
  
  uint8_t absSpeed = abs(speed);
  uint8_t direction = (speed >= 0) ? 1 : 0;
  
  if (wheel == FRONT_LEFT || wheel == FRONT_RIGHT) {
    if (wheel == FRONT_LEFT) {
      // out1,out2 controls front left wheel
      FRONT_BOARD_SERIAL.print("out1:");
      FRONT_BOARD_SERIAL.print(direction);
      FRONT_BOARD_SERIAL.print(",out2:");
      FRONT_BOARD_SERIAL.println(absSpeed);
    } else {
      // out3,out4 controls front right wheel
      FRONT_BOARD_SERIAL.print("out3:");
      FRONT_BOARD_SERIAL.print(direction);
      FRONT_BOARD_SERIAL.print(",out4:");
      FRONT_BOARD_SERIAL.println(absSpeed);
    }
  } 
  else if (wheel == REAR_LEFT || wheel == REAR_RIGHT) {
    if (wheel == REAR_LEFT) {
      // out1,out2 controls rear left wheel
      REAR_BOARD_SERIAL.print("out1:");
      REAR_BOARD_SERIAL.print(direction);
      REAR_BOARD_SERIAL.print(",out2:");
      REAR_BOARD_SERIAL.println(absSpeed);
    } else {
      // out3,out4 controls rear right wheel
      REAR_BOARD_SERIAL.print("out3:");
      REAR_BOARD_SERIAL.print(direction);
      REAR_BOARD_SERIAL.print(",out4:");
      REAR_BOARD_SERIAL.println(absSpeed);
    }
  }
}

// Convenience functions for each wheel
void moveFrontLeft(int8_t speed) {
  moveWheel(FRONT_LEFT, speed);
}

void moveFrontRight(int8_t speed) {
  moveWheel(FRONT_RIGHT, speed);
}

void moveRearLeft(int8_t speed) {
  moveWheel(REAR_LEFT, speed);
}

void moveRearRight(int8_t speed) {
  moveWheel(REAR_RIGHT, speed);
}

// Stop all wheels
void stopAllWheels() {
  moveFrontLeft(0);
  moveFrontRight(0);
  moveRearLeft(0);
  moveRearRight(0);
}

#endif
