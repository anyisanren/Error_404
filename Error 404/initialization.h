#ifndef INITIALIZATION_H
#define INITIALIZATION_H

// ===== 串口配置 =====
#define SERIAL_BAUD 9600

// ===== 电机引脚配置 =====
// 左轮电机
#define LEFT_MOTOR_PIN1 22
#define LEFT_MOTOR_PIN2 23
#define LEFT_MOTOR_PWM 3

// 右轮电机
#define RIGHT_MOTOR_PIN1 24
#define RIGHT_MOTOR_PIN2 25
#define RIGHT_MOTOR_PWM 4


// ===== 电机速度范围 =====
#define MAX_SPEED 255
#define MIN_SPEED 0

// ===== 函数声明 =====
void initSerial();
void initMotors();
void initPS2();

// ===== 初始化串口 =====
void initSerial() {
  Serial.begin(SERIAL_BAUD);
  delay(500);
  Serial.println("=== Arduino Mega2560 初始化开始 ===");
  Serial.println("串口初始化完成");
}

// ===== 初始化电机 =====
void initMotors() {
  // 设置左轮电机引脚
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  analogWrite(LEFT_MOTOR_PWM, 0);
  
  // 设置右轮电机引脚
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
  analogWrite(RIGHT_MOTOR_PWM, 0);
  
  
}

// ===== 初始化PS2手柄 =====
void initPS2() {
  pinMode(PS2_CLK, INPUT_PULLUP);
  pinMode(PS2_CMD, OUTPUT);
  pinMode(PS2_SEL, OUTPUT);
  pinMode(PS2_DAT, INPUT_PULLUP);
  
  digitalWrite(PS2_CMD, HIGH);
  digitalWrite(PS2_SEL, HIGH);
  
  Serial.println("PS2手柄引脚初始化完成");
}

#endif
