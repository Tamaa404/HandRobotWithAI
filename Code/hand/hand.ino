#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver myServo = Adafruit_PWMServoDriver();

#define MIN 150
#define MID 375
#define MAX 600

uint8_t numberOfServos = 5;

void setup() {
  Serial.begin(9600);
  myServo.begin();
  myServo.setPWMFreq(60);
  delay(10);
}

void setAllServos(int position) {
  for (uint8_t i = 0; i < numberOfServos; i++) {
    myServo.setPWM(i, 0, position);
  }
}

void loop() {
  if (Serial.available()) {
    char input = Serial.read();
    if (input == '1') {
      setAllServos(MIN);
    } else if (input == '2') {
      setAllServos(MID);
    } else if (input == '3') {
      setAllServos(MAX);
    }
    delay(500); // Delay to allow servo movements to complete
  }
}
