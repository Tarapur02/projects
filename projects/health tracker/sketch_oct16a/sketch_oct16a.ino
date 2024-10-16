#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

const int SENSOR_PIN = 23; // Arduino pin connected to DS18B20 sensor's DQ pin
OneWire oneWire(SENSOR_PIN); // Setup a OneWire instance
DallasTemperature sensors(&oneWire); // Pass oneWire to DallasTemperature library
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

double alpha = 0.75;
int period = 20;
double refresh = 0.0;
float tempCelsius; // Temperature in Celsius

void setup() {
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Health Tracker");
  lcd.setCursor(0, 1);
  lcd.print("* Band");
  delay(4000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Getting Data...");
  delay(5000);
  lcd.clear();
  
  // Initialize Serial communication
  Serial.begin(9600);
  
  // Initialize DallasTemperature sensor
  sensors.begin();
}

void loop() {
  static double oldValue = 0;
  int beat = analogRead(A0);
  double value = alpha * oldValue + (1 - alpha) * beat;
  refresh = value - oldValue;
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  tempCelsius = sensors.getTempCByIndex(0); // Read temperature in Celsius

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Heart: ");
  lcd.print(beat);

  // Temperature Reading from a different sensor on A2
  int sensorValue = analogRead(A2);
  float voltage = sensorValue * (5.0 / 1023.0);
  float temperature = voltage * 200;

  // Print temperature to Serial Monitor
  Serial.print("Voltage: ");
  Serial.println(voltage);
  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Display temperature on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print((char)223); // Degree symbol
  lcd.print("C");

  oldValue = value;
  delay(period * 20);
}
