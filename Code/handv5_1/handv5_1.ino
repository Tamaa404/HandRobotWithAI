#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>

// Define Bluetooth pins for SoftwareSerial
SoftwareSerial bluetooth(10, 11); // RX, TX

Adafruit_PWMServoDriver myServo = Adafruit_PWMServoDriver();
uint8_t numberOfServos = 5;

// Define unique position mappings for each servo
int servo1Positions[] = {150, 260, 360, 430, 520, 590};
int servo2Positions[] = {150, 250, 350, 420, 520, 590};
int servo3Positions[] = {150, 270, 370, 450, 520, 590};
int servo4Positions[] = {160, 290, 370, 450, 520, 590};
int servo5Positions[] = {155, 230, 330, 450, 570, 590};

int* servoPositions[] = {servo1Positions, servo2Positions, servo3Positions, servo4Positions, servo5Positions};

// Define a struct to hold servo configurations for each letter
struct ServoConfig {
  int positions[5];  // Array to store the level for each servo
};

// Define an array to store configurations for letters 'a' to 'z' (26 letters)
ServoConfig letterConfigs[26] = {
  {{4, 6, 6, 6, 6}},  // Configuration for 'a'
  {{6, 1, 1, 1, 1}},  // Configuration for 'b'
  {{3, 4, 4, 4, 4}},  // Configuration for 'c'
  {{5, 1, 5, 5, 5}},  // Configuration for 'd'
  {{5, 5, 5, 5, 5}},  // Configuration for 'e'
  {{6, 6, 1, 1, 1}},  // f
  {{1, 3, 6, 6, 6}},  // g
  {{6, 1, 1, 6, 6}},  // h
  {{6, 6, 6, 6, 1}},  // i
  {{6, 6, 6, 6, 1}},  //  j tidak bisa di praktekkan
  {{4, 1, 1, 6, 6}},  // k
  {{1, 1, 6, 6, 6}},  // l
  {{6, 3, 3, 3, 6}},  // m
  {{6, 3, 3, 6, 6}},  // n
  {{5, 5, 5, 5, 5}},  // o
  {{5, 2, 5, 6, 6}},  // p
  {{5, 5, 2, 6, 6}},  // q
  {{6, 1, 1, 6, 6}},  // r tidak bisa di praktekkan
  {{6, 6, 6, 6, 6}},  // s
  {{1, 5, 6, 6, 6}},  // t
  {{1, 6, 6, 6, 1}},  // y
  
  // Add additional configurations for letters 'f' to 'z' as needed
};

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);  // Start Bluetooth communication
  myServo.begin();
  myServo.setPWMFreq(60);
  delay(10);
  
  Serial.println("Enter values for each servo in format: 1-2-3-4-5 (values from 1 to 6), or type 'a', 'b', 'c', etc.");
  bluetooth.println("Bluetooth connected! Enter commands as 'a', 'b', 'c' or custom: 1-2-3-4-5.");
}

void setServoPositions(int positions[]) {
  for (uint8_t i = 0; i < numberOfServos; i++) {
    myServo.setPWM(i, 0, positions[i]);
  }
}

void setLetterConfig(char letter) {
  int letterIndex = letter - 'a';  // Convert letter to index (0 for 'a', 1 for 'b', etc.)
  if (letterIndex >= 0 && letterIndex < 26) {
    int positions[numberOfServos];
    ServoConfig config = letterConfigs[letterIndex];
    
    // Map level values to PWM values using servoPositions array
    for (int i = 0; i < numberOfServos; i++) {
      int level = config.positions[i];
      if (level >= 1 && level <= 6) {
        positions[i] = servoPositions[i][level - 1]; // Convert level to PWM value
      } else {
        Serial.println("Error: Level values must be between 1 and 6.");
        bluetooth.println("Error: Level values must be between 1 and 6.");
        return;
      }
    }
    
    setServoPositions(positions);
    Serial.print("Servos set to configuration for '");
    Serial.print(letter);
    Serial.println("'.");
    bluetooth.print("Servos set to configuration for '");
    bluetooth.print(letter);
    bluetooth.println("'.");
  } else {
    Serial.println("Error: Configuration not found for this letter.");
    bluetooth.println("Error: Configuration not found for this letter.");
  }
}

void setCustomConfig(String input) {
  int positions[numberOfServos];
  int index = 0;
  int start = 0;
  int end = input.indexOf('-');
  
  // Split input by '-' and convert each part to an integer level
  while (end != -1 && index < numberOfServos) {
    int level = input.substring(start, end).toInt();
    if (level >= 1 && level <= 6) {
      positions[index] = servoPositions[index][level - 1];
      index++;
    } else {
      Serial.println("Error: Input values must be between 1 and 6.");
      bluetooth.println("Error: Input values must be between 1 and 6.");
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
      bluetooth.println("Error: Input values must be between 1 and 6.");
      return;
    }
  }

  // Ensure we have the correct number of inputs
  if (index == numberOfServos) {
    setServoPositions(positions);
    Serial.println("Servos set to specified custom positions.");
    bluetooth.println("Servos set to specified custom positions.");
  } else {
    Serial.println("Error: Please enter 5 values separated by '-' for each servo.");
    bluetooth.println("Error: Please enter 5 values separated by '-' for each servo.");
  }
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() == 1 && isalpha(input[0])) {
      // If input is a single letter, set the predefined configuration
      char letter = tolower(input[0]);
      setLetterConfig(letter);
    } else if (input.indexOf('-') != -1) {
      // If input contains '-', treat it as a custom configuration
      setCustomConfig(input);
    } else {
      Serial.println("Error: Please enter a single letter (a-z) or a custom configuration in the format: 1-2-3-4-5.");
    }
    
    delay(2000); // Short delay to allow processing time
  }

  if (bluetooth.available()) {
    String input = bluetooth.readStringUntil('\n');
    input.trim();

    if (input.length() == 1 && isalpha(input[0])) {
      char letter = tolower(input[0]);
      setLetterConfig(letter);
    } else if (input.indexOf('-') != -1) {
      setCustomConfig(input);
    } else {
      bluetooth.println("Error: Please enter a single letter (a-z) or a custom configuration in the format: 1-2-3-4-5.");
    }

    delay(2000);
  }
}
