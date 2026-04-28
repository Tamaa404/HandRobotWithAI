#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver myServo = Adafruit_PWMServoDriver();

#define MIN 150
#define MAX 600

uint8_t numberOfServos = 5;

void setup() {
  Serial.begin(9600);
  myServo.begin();
  myServo.setPWMFreq(60);
  delay(10);
  Serial.println("Enter values for each servo in format: 150-150-150-150-150");
}

void setServoPositions(int positions[]) {
  for (uint8_t i = 0; i < numberOfServos; i++) {
    myServo.setPWM(i, 0, positions[i]);
  }
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any whitespace or newline characters
    
    int positions[numberOfServos];
    int index = 0;

    // Split input by '-' and convert each part to an integer
    int start = 0;
    int end = input.indexOf('-');
    while (end != -1 && index < numberOfServos) {
      positions[index++] = input.substring(start, end).toInt();
      start = end + 1;
      end = input.indexOf('-', start);
    }
    // Add the last segment
    if (index < numberOfServos) {
      positions[index++] = input.substring(start).toInt();
    }

    // Ensure we have the correct number of inputs
    if (index == numberOfServos) {
      setServoPositions(positions);
      Serial.println("Servos set to specified positions.");
    } else {
      Serial.println("Error: Please enter 5 values separated by '-' for each servo.");
    }

    delay(500); // Short delay to allow processing time
  }
}
