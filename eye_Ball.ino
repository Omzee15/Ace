#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  100  // Minimum pulse length for 0°
#define SERVOMAX  500  // Maximum pulse length for 180°

#define Yarm 0
#define Xarm 1
#define top_left_eyelid 2
#define bottom_left_eyelid 3
#define top_right_eyelid 4
#define bottom_right_eyelid 5

void setup() {
    Serial.begin(115200);
    Wire.begin();
    pwm.begin();
    pwm.setPWMFreq(50); // 50Hz for servos

    Serial.println("Starting synchronized slow blink...");
}

void loop() {
    eye_open();
    delay(500);
    rotate_X_axis();
    delay(500);
    eye_center();
    delay(500);
    rotate_Y_axis();
    rotate_Y_axis();
    delay(500);
    eye_close();
    delay(500);
    //slow_blink(); // Blink the eyes
    //delay(1000);  // Wait before blinking again
}

// Function to move servo to a specific angle
void setServoAngle(int channel, int angle) {
    int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
    pwm.setPWM(channel, 0, pulse);
}

// Function for smooth synchronized blinking
void slow_blink() {
    for (int angle = 55; angle >= 2; angle--) {  
        setServoAngle(top_left_eyelid, angle);
        setServoAngle(top_right_eyelid, angle);
        setServoAngle(bottom_left_eyelid, 36 + (120 - angle));  
        setServoAngle(bottom_right_eyelid, 36 + (120 - angle));
        delay(50); // Adjust for smoothness
    }

    delay(100); // Keep eyes closed for a brief moment

    for (int angle = 2; angle <= 55; angle++) {  
        setServoAngle(top_left_eyelid, angle);
        setServoAngle(top_right_eyelid, angle);
        setServoAngle(bottom_left_eyelid, 36 + (120 - angle));  
        setServoAngle(bottom_right_eyelid, 36 + (120 - angle));
        delay(50); // Adjust for smoothness
    }

    Serial.println("Blink completed.");
}
void eye_close() {
    for (int angle = 55; angle >= 2; angle--) {  
        setServoAngle(top_left_eyelid, angle);
        setServoAngle(top_right_eyelid, angle);
        setServoAngle(bottom_left_eyelid, 36 + (120 - angle));  
        setServoAngle(bottom_right_eyelid, 36 + (120 - angle));
        delay(50); // Adjust for smoothness
    }
}

void eye_open() {
    for (int angle = 2; angle <= 55; angle++) {  
        setServoAngle(top_left_eyelid, angle);
        setServoAngle(top_right_eyelid, angle);
        setServoAngle(bottom_left_eyelid, 36 + (120 - angle));  
        setServoAngle(bottom_right_eyelid, 36 + (120 - angle));
        delay(50); // Adjust for smoothness
    }
}

void rotate_X_axis() {
    for (int angle = 120; angle >= 60; angle--) {
        setServoAngle(Xarm, angle);
        delay(15);
    }
    for (int angle = 60; angle <= 120; angle++) {
        setServoAngle(Xarm, angle);
        delay(15);
    }
}

void rotate_Y_axis() {
    for (int angle = 60; angle <= 120; angle++) {
        setServoAngle(Yarm, angle);
        delay(15);
    }
    for (int angle = 120; angle >= 60; angle--) {
        setServoAngle(Yarm, angle);
        delay(15);
    }
    setServoAngle(Yarm, 90);
}

void eye_center() {
    setServoAngle(Xarm, 90);
    setServoAngle(Yarm, 90);
}
