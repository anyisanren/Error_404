#include "initialization.h"
#include "movement.h"

void setup() {
  // 初始化串口
  initSerial();
  
  // 初始化电机
  initMotors();
  
  // 初始化运动控制
  initMovement();
  
  // 初始化PS2手柄
  // initPS2();
  
  Serial.println("=== 所有模块初始化完成 ===\n");
}

// 测试状态机
enum TestState {
  TEST_INIT,
  TEST_FRONT_LEFT,
  TEST_FRONT_RIGHT,
  TEST_REAR_LEFT,
  TEST_REAR_RIGHT,
  TEST_ALL_FORWARD,
  TEST_ALL_BACKWARD,
  TEST_COMPLETE
};

static TestState testState = TEST_INIT;
static unsigned long stateStartTime = 0;
static const unsigned long TEST_DURATION = 2000;  // 每个测试2秒

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - stateStartTime;
  
  switch (testState) {
    case TEST_INIT:
      Serial.println("\n=== 开始运动测试 ===");
      Serial.println("测试1: 前左轮前进");
      moveFrontLeft(150);
      stateStartTime = currentTime;
      testState = TEST_FRONT_LEFT;
      break;
      
    case TEST_FRONT_LEFT:
      if (elapsed >= TEST_DURATION) {
        Serial.println("测试1完成，停止");
        moveFrontLeft(0);
        delay(500);
        
        Serial.println("测试2: 前右轮前进");
        moveFrontRight(150);
        stateStartTime = currentTime;
        testState = TEST_FRONT_RIGHT;
      }
      break;
      
    case TEST_FRONT_RIGHT:
      if (elapsed >= TEST_DURATION) {
        Serial.println("测试2完成，停止");
        moveFrontRight(0);
        delay(500);
        
        Serial.println("测试3: 后左轮前进");
        moveRearLeft(150);
        stateStartTime = currentTime;
        testState = TEST_REAR_LEFT;
      }
      break;
      
    case TEST_REAR_LEFT:
      if (elapsed >= TEST_DURATION) {
        Serial.println("测试3完成，停止");
        moveRearLeft(0);
        delay(500);
        
        Serial.println("测试4: 后右轮前进");
        moveRearRight(150);
        stateStartTime = currentTime;
        testState = TEST_REAR_RIGHT;
      }
      break;
      
    case TEST_REAR_RIGHT:
      if (elapsed >= TEST_DURATION) {
        Serial.println("测试4完成，停止");
        moveRearRight(0);
        delay(500);
        
        Serial.println("测试5: 所有轮子前进");
        moveFrontLeft(200);
        moveFrontRight(200);
        moveRearLeft(200);
        moveRearRight(200);
        stateStartTime = currentTime;
        testState = TEST_ALL_FORWARD;
      }
      break;
      
    case TEST_ALL_FORWARD:
      if (elapsed >= TEST_DURATION) {
        Serial.println("测试5完成，停止");
        stopAllWheels();
        delay(500);
        
        Serial.println("测试6: 所有轮子后退");
        moveFrontLeft(-200);
        moveFrontRight(-200);
        moveRearLeft(-200);
        moveRearRight(-200);
        stateStartTime = currentTime;
        testState = TEST_ALL_BACKWARD;
      }
      break;
      
    case TEST_ALL_BACKWARD:
      if (elapsed >= TEST_DURATION) {
        Serial.println("测试6完成，停止");
        stopAllWheels();
        delay(500);
        
        Serial.println("\n=== 所有测试完成 ===");
        testState = TEST_COMPLETE;
      }
      break;
      
    case TEST_COMPLETE:
      // 测试完成，保持停止状态
      break;
  }
}
