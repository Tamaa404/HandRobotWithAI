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
  {{6, 6, 1, 1, 1}},  // Configuration for 'f'
  {{1, 3, 6, 6, 6}},  // Configuration for 'g'
  {{6, 1, 1, 6, 6}},  // Configuration for 'h'
  {{6, 6, 6, 6, 1}},  // Configuration for 'i'
  {{6, 6, 6, 6, 1}},  // Configuration for 'j'
  {{4, 1, 1, 6, 6}},  // Configuration for 'k'
  {{1, 1, 6, 6, 6}},  // Configuration for 'l'
  {{6, 3, 3, 3, 6}},  // Configuration for 'm'
  {{6, 3, 3, 6, 6}},  // Configuration for 'n'
  {{5, 5, 5, 5, 5}},  // Configuration for 'o'
  {{5, 2, 5, 6, 6}},  // Configuration for 'p'
  {{5, 5, 2, 6, 6}},  // Configuration for 'q'
  {{6, 1, 1, 6, 6}},  // Configuration for 'r'
  {{6, 6, 6, 6, 6}},  // Configuration for 's'
  {{1, 5, 6, 6, 6}},  // Configuration for 't'
  {{1, 6, 6, 6, 1}},  // Configuration for 'u'
  {{2, 5, 5, 5, 5}},  // Configuration for 'v'
  {{3, 4, 4, 4, 4}},  // Configuration for 'w'
  {{4, 1, 6, 6, 6}},  // Configuration for 'x'
  {{5, 5, 5, 6, 6}},  // Configuration for 'y'
  {{6, 6, 6, 1, 1}}   // Configuration for 'z'
};

// Queue to hold commands (letters or custom configurations)
String commandQueue = "";

// Helper functions
void setServoPositions(int positions[]) {
  for (uint8_t i = 0; i < numberOfServos; i++) {
    myServo.setPWM(i, 0, positions[i]);
  }
}

void setLetterConfig(char letter) {
  int letterIndex = letter - 'a';
  if (letterIndex >= 0 && letterIndex < 26) {
    int positions[numberOfServos];
    ServoConfig config = letterConfigs[letterIndex];
    for (int i = 0; i < numberOfServos; i++) {
      int level = config.positions[i];
      positions[i] = servoPositions[i][level - 1];
    }
    setServoPositions(positions);
    Serial.print("Performing letter: ");
    Serial.println(letter);
    bluetooth.print("Performing letter: ");
    bluetooth.println(letter);
  }
}

void setCustomConfig(String config) {
  int positions[numberOfServos];
  int index = 0;
  for (int i = 0; i < config.length(); i++) {
    if (isdigit(config[i])) {
      int level = config[i] - '0';
      if (level >= 1 && level <= 6 && index < numberOfServos) {
        positions[index] = servoPositions[index][level - 1];
        index++;
      }
    }
  }
  if (index == numberOfServos) {
    setServoPositions(positions);
    Serial.print("Performing custom configuration: ");
    Serial.println(config);
    bluetooth.print("Performing custom configuration: ");
    bluetooth.println(config);
  } else {
    Serial.println("Invalid custom configuration");
    bluetooth.println("Invalid custom configuration");
  }
}

void handleCommandQueue() {
  bool processedCommands = false;

  while (commandQueue.length() > 0) {
    // Trim leading whitespace
    commandQueue.trim();

    if (commandQueue.length() == 0) {
      break; // Exit if the queue becomes empty after trimming
    }

    // Process the first character
    char currentChar = commandQueue[0];

    // Handle spaces with a delay
    if (currentChar == ' ') {
      delay(1000); // Delay for space (1000ms = 1 second)
    } 
    // Check for single letters
    else if (isalpha(currentChar)) {
      setLetterConfig(currentChar); // Perform letter action
      delay(1000); // Delay after processing the letter
    } 
    // Check for custom configuration (e.g., 6-6-6-6-6)
    else if (currentChar == '-' || isdigit(currentChar)) {
      int spaceIndex = commandQueue.indexOf(' ');
      String config = (spaceIndex != -1) ? commandQueue.substring(0, spaceIndex) : commandQueue;
      setCustomConfig(config); // Perform custom action
      delay(1000); // Delay after processing the configuration

      // Remove the processed configuration
      commandQueue = (spaceIndex != -1) ? commandQueue.substring(spaceIndex + 1) : "";
      processedCommands = true; // Mark as processed
      continue; // Skip the rest of the loop for custom config
    } 
    // Handle invalid characters
    else {
      Serial.print("Invalid command: ");
      Serial.println(currentChar);
      bluetooth.print("Invalid command: ");
      bluetooth.println(currentChar);
    }

    // Remove the processed character
    commandQueue = commandQueue.substring(1);
    processedCommands = true; // Mark as processed
  }

  // Reset to default position only if any commands were processed
  if (processedCommands) {
    resetToDefaultPosition();
  }
}

// Setup and loop
void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  myServo.begin();
  myServo.setPWMFreq(60);
  delay(10);
  
  Serial.println("Enter letters (e.g., abc) or custom configurations (e.g., 6-6-6-6-6).");
  bluetooth.println("Bluetooth connected! Enter letter sequence or custom configurations.");
}

void loop() {
  // Check Serial input
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    commandQueue += input + " "; // Append new commands to queue
  }

  // Check Bluetooth input
  if (bluetooth.available()) {
    String input = bluetooth.readStringUntil('\n');
    input.trim();
    commandQueue += input + " "; // Append new commands to queue
  }

  // Process the command queue
  handleCommandQueue();
}

void resetToDefaultPosition() {
  int defaultPositions[numberOfServos];
  for (int i = 0; i < numberOfServos; i++) {
    defaultPositions[i] = servoPositions[i][0]; // Assuming the first position in each servo array is the default
  }
  setServoPositions(defaultPositions);
  Serial.println("Servos reset to default position.");
  bluetooth.println("Servos reset to default position.");
}
