#include <SoftwareSerial.h>

const String PHONE1 = "+916362272327"; // Phone number for flame sensor alerts
const String PHONE2 = "+919141008624"; // Phone number for wet bin alerts
const String PHONE3 = "+919380062677"; // Phone number for dry bin alerts

#define rxPin 2
#define txPin 3

SoftwareSerial sim800L(rxPin, txPin);

int flameSensor = 5;
int wetBin = 6;
int dryBin = 7;
int flameDetected;
int wetBinDetected;
int dryBinDetected;

#define buzzerPin 4

void setup() {
  Serial.begin(115200);
  sim800L.begin(9600);

  pinMode(flameSensor, INPUT);
  pinMode(wetBin, INPUT);
  pinMode(dryBin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  Serial.println("Initializing SIM800L...");
  sim800L.println("AT");
  delay(1000);
  sim800L.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
}

void loop() {
  while (sim800L.available()) {
    Serial.println(sim800L.readString());
  }

  flameDetected = digitalRead(flameSensor);
  wetBinDetected = digitalRead(wetBin);
  dryBinDetected = digitalRead(dryBin);

  Serial.print("Flame: ");
  Serial.println(flameDetected);
  Serial.print("Wet bin: ");
  Serial.println(wetBinDetected);
  Serial.print("Dry bin: ");
  Serial.println(dryBinDetected);

  if (flameDetected == 0) {
    handleAlert("Dustbin is full. Take action immediately. Location: http://maps.app.goo.gl/FLAME_LOCATION", PHONE1);
  }
  if (wetBinDetected == 0) {
    handleAlert("Wet bin is full. Take action immediately. Location: https://maps.app.goo.gl/WET_BIN_LOCATION", PHONE2);
  }
  if (dryBinDetected == 0) {
    handleAlert("Dry bin is full. Take action immediately. Location: https://maps.app.goo.gl/DRY_BIN_LOCATION", PHONE3);
  }
  
  delay(5000); // Delay before the next check
}

void handleAlert(String message, String phone) {
  digitalWrite(buzzerPin, HIGH);
  Serial.println(message);
  sendSMS(message, phone);
  makeCall(phone);
  digitalWrite(buzzerPin, LOW);
}

void sendSMS(String text, String phone) {
  Serial.println("Sending SMS...");
  sim800L.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  sim800L.print("AT+CMGS=\"");
  sim800L.print(phone);
  sim800L.println("\"");
  delay(1000);
  sim800L.print(text);
  delay(100);
  sim800L.write(0x1A); // Ctrl+Z to send SMS
  delay(5000); // Wait for message to send
}

void makeCall(String phone) {
  Serial.println("Calling...");
  sim800L.print("ATD");
  sim800L.print(phone);
  sim800L.println(";");
  delay(20000); // 20 seconds delay during call
  sim800L.println("ATH"); // Hang up call
  delay(1000); // 1 second delay after hang up
}
