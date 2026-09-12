#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <esp32Servo.h>


// ประกาศ extern เพื่อให้ sensor.cpp รู้จักตัวแปรที่ถูกสร้างและให้ค่าไว้ใน main.cpp
extern const int pingPin;
extern int inPin;
extern int ledPin;
extern int digitalPin;
extern int val;
extern int buzzer;
extern int servoPin;

extern Servo doorServo;

extern const int OPEN_ANGLE;
extern const int CLOSED_ANGLE;
extern bool isDoorOpen;

extern const int DISTANCE_THRESHOLD;
extern const int numPasswords;
extern String savedPasswords[];

extern unsigned long doorOpenTime;
extern const unsigned long autoCloseDelay;

// ประกาศฟังก์ชันสำหรับเรียกใช้
void setupSensors();
void openDoor();
void closeDoor();
long readDistance();
void handleMotionSensor();
void checkPasswordSystem(long cm);
void checkAutoClose();

#endif