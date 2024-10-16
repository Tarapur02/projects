#include <Servo.h>

// Define sensor pins
int LS = 2;
int RS = 3;
int LE = 4;
int ES = 5;
int VS = 7;

// Create servo objects
Servo servo1;
Servo servo2;

// Variables to store the last update time
unsigned long lastUpdateTime1 = 0;
unsigned long lastUpdateTime2 = 0;

// Interval for delay (1 minute)
const unsigned long interval = 60000; // 1 minute in milliseconds

// Variables to store current positions of servos
int pos1 = 90; // Initial position for servo1
int pos2 = 90; // Initial position for servo2

void setup() {
  // Set sensor pins as inputs
  pinMode(LS, INPUT);
  pinMode(RS, INPUT);
  pinMode(LE, INPUT);
  pinMode(ES, INPUT);
  pinMode(VS, INPUT);

  // Attach servos to their respective pins
  servo1.attach(9);
  servo2.attach(10);

  // Move servos to initial positions
  servo1.write(pos1);
  servo2.write(pos2);
}

void loop() {
  // Read sensor values
  int lsValue = digitalRead(LS);
  int rsValue = digitalRead(RS);
  int leValue = digitalRead(LE);
  int esValue = digitalRead(ES);
  int vsValue = digitalRead(VS);

  // Get the current time
  unsigned long currentTime = millis();

  // Update servo1 position if 1 minute has passed
  if (currentTime - lastUpdateTime1 > interval) {
    // Determine new position for servo1 based on sensor values
    if (lsValue == 0 && rsValue == 1) {
      pos1 = 10;
    } else if (lsValue == 1 && rsValue == 0) {
      pos1 = 170;
    } else {
      pos1 = 90; // Default position
    }

    // Move servo1 to new position
    servo1.write(pos1);

    // Update last update time for servo1
    lastUpdateTime1 = currentTime;
  }

  // Update servo2 position if 1 minute has passed
  if (currentTime - lastUpdateTime2 > interval) {
    // Determine new position for servo2 based on sensor values
    if (leValue == 1 && esValue == 1) {
      pos2 = 10;
    } else if (leValue == 0 && esValue == 0) {
      pos2 = 170;
    } else {
      pos2 = 120; // Default position
    }

    // Move servo2 to new position
    servo2.write(pos2);

    // Update last update time for servo2
    lastUpdateTime2 = currentTime;
  }

  // Short delay for debouncing sensor readings
  delay(500);
}
