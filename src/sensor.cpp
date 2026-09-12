#include "sensor.h"

// ฟังก์ชันตั้งค่าเริ่มต้น
void setupSensors() {
  Serial.begin(115200);
  pinMode(digitalPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  ESP32PWM::allocateTimer(0);

  doorServo.setPeriodHertz(50);
  doorServo.attach(servoPin, 500, 2400); 
  doorServo.write(CLOSED_ANGLE); 
  isDoorOpen = false;
  
  Serial.println("System Ready. Waiting for car to approach...");
}

// ฟังก์ชันเปิดประตู
void openDoor() {
  if (isDoorOpen) return; 
  
  for (int pos = CLOSED_ANGLE; pos >= OPEN_ANGLE; pos--) {
    doorServo.write(pos);
    delay(15); 
  }
  isDoorOpen = true;
  Serial.println(">>> Door Opened <<<");
}

// ฟังก์ชันปิดประตู
void closeDoor() {
  if (!isDoorOpen) return; 
  
  for (int pos = OPEN_ANGLE; pos <= CLOSED_ANGLE; pos++) {
    doorServo.write(pos);
    delay(15); 
  }
  isDoorOpen = false;
  Serial.println(">>> Door Closed <<<");
}

// ฟังก์ชันอ่านค่า Ultrasonic และคืนค่าระยะทางกลับไป
long readDistance() {
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  
  pinMode(inPin, INPUT);
  long duration = pulseIn(inPin, HIGH, 30000);
  long cm = duration / 58;

  // แสดงระยะทางใน Serial Monitor ตลอดเวลา
  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");

  // เช็คว่ามีรถขวางอยู่ไหม เพื่อยืดเวลาไม่ให้ประตูปิดชนรถตอนที่ประตูเปิดอยู่
  if (cm > 0 && cm <= DISTANCE_THRESHOLD) {
    if (isDoorOpen) {
      doorOpenTime = millis(); 
    }
  }
  return cm;
}

// ฟังก์ชันอ่านค่าเซ็นเซอร์เคลื่อนไหว
void handleMotionSensor() {
  val = digitalRead(digitalPin);
  if (val == 0) {
    digitalWrite(ledPin, LOW); 
  } else {
    digitalWrite(ledPin, HIGH); 
  }
}

// ฟังก์ชันตรวจสอบรหัสผ่าน
void checkPasswordSystem(long cm) {
  if (Serial.available() > 0) {
    String inputCode = Serial.readStringUntil('\n');
    inputCode.trim(); 

    bool isPasswordCorrect = false;
    for (int i = 0; i < numPasswords; i++) {
      if (inputCode == savedPasswords[i]) {
        isPasswordCorrect = true;
        break; 
      }
    }

    if (isPasswordCorrect) {
      if (cm > 0 && cm <= DISTANCE_THRESHOLD) {
        Serial.println("Password Correct & Car detected! Access Granted.");
        digitalWrite(buzzer, HIGH); delay(200); digitalWrite(buzzer, LOW);
        openDoor();               
        doorOpenTime = millis();   
      } else {
        Serial.println("Password Correct, BUT NO CAR detected! Access Denied.");
        for(int i = 0; i < 3; i++) {
          digitalWrite(buzzer, HIGH); delay(100); digitalWrite(buzzer, LOW); delay(100);
        }
      }
    } else {
      Serial.println("Wrong Password! Access Denied.");
      for(int i = 0; i < 3; i++) {
        digitalWrite(buzzer, HIGH); delay(100); digitalWrite(buzzer, LOW); delay(100);
      }
    }
  }
}

// ฟังก์ชันตรวจสอบการปิดประตูอัตโนมัติ
void checkAutoClose() {
  if (isDoorOpen && (millis() - doorOpenTime >= autoCloseDelay)) {
    closeDoor(); 
  }
}