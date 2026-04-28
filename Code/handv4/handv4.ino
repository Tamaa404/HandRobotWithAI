#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver myServo = Adafruit_PWMServoDriver();

uint8_t numberOfServos = 5;

void setup() {
  Serial.begin(9600);
  myServo.begin();
  myServo.setPWMFreq(60);
  delay(10);
  Serial.println("Enter values for each servo in format: 1-2-3-4-5 (values from 1 to 6)");
}

// Define unique position mappings for each servo
int servo1Positions[] = {150, 260, 360, 430, 520, 590};
int servo2Positions[] = {150, 250, 350, 420, 520, 590};
int servo3Positions[] = {150, 290, 390, 450, 520, 590};
int servo4Positions[] = {160, 290, 390, 450, 520, 590};
int servo5Positions[] = {155, 190, 230, 450, 570, 590};

int* servoPositions[] = {servo1Positions, servo2Positions, servo3Positions, servo4Positions, servo5Positions};

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
      int level = input.substring(start, end).toInt();
      if (level >= 1 && level <= 6) {
        positions[index] = servoPositions[index][level - 1]; // Map to specific servo position
        index++;
      } else {
        Serial.println("Error: Input values must be between 1 and 6.");
        return;
      }
      start = end + 1;
      end = input.indexOf('-', start);
    }
    // Add the last segment
    if (index < numberOfServos) {
      int level = input.substring(start).toInt();
      if (level >= 1 && level <= 6) {
        positions[index] = servoPositions[index][level - 1];
        index++;
      } else {
        Serial.println("Error: Input values must be between 1 and 6.");
        return;
      }
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
