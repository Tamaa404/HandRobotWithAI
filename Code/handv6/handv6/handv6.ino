#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Initialize the PWM servo driver
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_MIN 550  // Adjusted for inverted movement
#define SERVO_MAX 150  // Adjusted for inverted movement

// Define servo channels
#define SERVO_THUMB 0
#define SERVO_INDEX 1
#define SERVO_MIDDLE 2
#define SERVO_RING 3
#define SERVO_PINKY 4

String receivedString = "";  // Buffer for received string
int stringLength = 6;        // Expected length of the command (including '$')
bool stringStart = false;
int servoStates[5] = {1, 1, 1, 1, 1};  // Stores the state of each servo

void setup() {
  Serial.begin(115200);  // Increased baud rate for faster communication
  pwm.begin();
  pwm.setPWMFreq(60);  // Set frequency to 60 Hz for servos
  delay(10);

  // Print ready message
  Serial.println("Arduino is ready to receive data!");
}

void loop() {
  receiveCommand();
  updateServos();
}

void receiveCommand() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '$') {  // Start of a new command
      stringStart = true;
      receivedString = "";  // Reset the string buffer
    }

    if (stringStart) {
      receivedString += c;

      // Check if the complete command has been received
      if (receivedString.length() == stringLength) {
        stringStart = false;

        // Parse the command and extract servo states
        for (int i = 0; i < 5; i++) {
          servoStates[i] = receivedString.charAt(i + 1) - '0';  // Convert char to int
        }

        // Debugging: Print received string and parsed states
        Serial.print("Received command: ");
        Serial.println(receivedString);
        Serial.print("Servo states: ");
        for (int i = 0; i < 5; i++) {
          Serial.print(servoStates[i]);
          Serial.print(" ");
        }
        Serial.println();
      }
    }
  }
}

void updateServos() {
  // Update servo positions based on states
  moveServo(SERVO_THUMB, servoStates[0]);
  moveServo(SERVO_INDEX, servoStates[1]);
  moveServo(SERVO_MIDDLE, servoStates[2]);
  moveServo(SERVO_RING, servoStates[3]);
  moveServo(SERVO_PINKY, servoStates[4]);
}

void moveServo(int channel, int state) {
  int position = (state == 1) ? SERVO_MAX : SERVO_MIN;  // Inverted positions
  pwm.setPWM(channel, 0, position);
}
