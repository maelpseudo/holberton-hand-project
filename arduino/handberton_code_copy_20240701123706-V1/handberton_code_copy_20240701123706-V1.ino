#include <Arduino.h>

// Motor Pins (réorganisés selon les nouvelles exigences)
const int motorPins[5][2] = {
  {5, 6},   // Pouce (Doigt 1) - Moteur 1
  {3, 4},   // Index (Doigt 2) - Moteur 2
  {11, 12}, // Majeur (Doigt 3) - Moteur 3
  {8, 7},   // Annulaire (Doigt 4) - Moteur 4
  {9, 10}   // Petit Doigt (Doigt 5) - Moteur 5
};

// Flex Sensor Pins
const int flexSensorPins[5] = {A0, A1, A2, A3, A4};

// Standby Pin
const int standbyPin = 2;  // STBY connected to pin 2 of the Arduino

// Time variables for each motor in centiseconds (0.01 seconds)
const int maxTime[5] = {110, 400, 400, 350, 380}; // Example: 3.50 seconds
int currentTime[5] = {0, 0, 0, 0, 0};
int targetTime[5] = {0, 0, 0, 0, 0};
bool inCalibrationMode[5] = {false, false, false, false, false};
bool calibrationActive = false;

unsigned long previousMillis = 0;
unsigned long previousSecondMillis = 0;
const long interval = 1000; // Interval for sensor data update in milliseconds (1000ms = 1s)

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(motorPins[i][0], OUTPUT);
    pinMode(motorPins[i][1], OUTPUT);
  }
  pinMode(standbyPin, OUTPUT);

  // Activate the DRV8833 by setting the STBY pin high
  digitalWrite(standbyPin, HIGH);

  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Clean whitespace and newline character

    // Handle motor commands for calibration
    if (command.startsWith("motor")) {
      int motorIndex = command.substring(5, 6).toInt() - 1;
      String direction = command.substring(7); // Extract direction

      if (motorIndex >= 0 && motorIndex < 5) {
        controlMotor(motorIndex, direction);
        inCalibrationMode[motorIndex] = (direction != "stop");
      } else {
        Serial.println("Invalid motor index");
      }
    }
    // Handle finger commands for normal operation
    else if (command.startsWith("finger")) {
      int fingerIndex = command.substring(6, 7).toInt() - 1;
      String action = command.substring(8); // Extract action

      if (fingerIndex >= 0 && fingerIndex < 5) {
        if (fingerIndex == 0) { // Specific handling for finger1
          if (action.equals("open")) {
            targetTime[fingerIndex] = 70; // 0.70 seconds
          } else if (action.equals("close")) {
            targetTime[fingerIndex] = 110; // 1.10 seconds
          }
        } else {
          if (action.equals("open")) {
            targetTime[fingerIndex] = 0;
          } else if (action.equals("close")) {
            targetTime[fingerIndex] = maxTime[fingerIndex];
          }
        }
        inCalibrationMode[fingerIndex] = false; // Exit calibration mode
        calibrationActive = false; // Ensure we are out of calibration mode
      }
    }
    // Start calibration by resetting current times
    else if (command.equals("startCalibration")) {
      for (int i = 0; i < 5; i++) {
        currentTime[i] = 0;
        targetTime[i] = 0;
        inCalibrationMode[i] = true;
      }
      calibrationActive = true;
    }
  }

  // Move fingers to target positions if not in calibration mode
  for (int i = 0; i < 5; i++) {
    if (!inCalibrationMode[i] && !calibrationActive) {
      if (currentTime[i] < targetTime[i]) {
        controlFinger(i, "close");
        currentTime[i]++;
      } else if (currentTime[i] > targetTime[i]) {
        controlFinger(i, "open");
        currentTime[i]--;
      } else {
        controlFinger(i, "stop");
      }
    }
  }
  delay(10); // Delay to simulate time for motor to move

  // Send currentTime values over Serial every second
  if (currentMillis - previousSecondMillis >= interval) {
    previousSecondMillis = currentMillis;
    for (int i = 0; i < 5; i++) {
      Serial.print("CurrentTime");
      Serial.print(i + 1);
      Serial.print(":");
      Serial.println(currentTime[i]);
    }
  }
}

void controlMotor(int motorIndex, String direction) {
  int pinA = motorPins[motorIndex][0];
  int pinB = motorPins[motorIndex][1];

  if (direction.equals("up")) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
  } else if (direction.equals("down")) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
  } else if (direction.equals("stop")) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
  }
}

void controlFinger(int fingerIndex, String action) {
  int pinA = motorPins[fingerIndex][0];
  int pinB = motorPins[fingerIndex][1];

  if (action.equals("open")) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
  } else if (action.equals("close")) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
  } else if (action.equals("stop")) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
  }
}
