#include <DHT.h> // Include the DHT sensor library
#include <Wire.h> // Include the Wire library for I2C
#include <LiquidCrystal_I2C.h> // Include the LiquidCrystal_I2C library

#define DHTPIN 5 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22 (AM2302) sensor type

DHT dht(DHTPIN, DHTTYPE);

const int sensorPin = A0; // Soil sensor input at Analog Pin A0
const int relayPin = 6; // Relay connected to Digital Pin 6

// Set the LCD address to 0x27 for a 16x2 display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Wire.begin(); // Initialize I2C communication
  pinMode(sensorPin, INPUT);
  pinMode(relayPin, OUTPUT);

  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight

  dht.begin(); // Initialize the DHT sensor
}

void loop() {
  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error reading");
    lcd.setCursor(0, 1);
    lcd.print("DHT sensor");
    delay(2000);
    return;
  }

  // Display temperature and humidity on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print((char)223); // Degree symbol
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");

  // Read soil moisture sensor
  int moisture = analogRead(sensorPin);
  int moisturePercent = map(moisture, 0, 1023, 0, 100); // Map moisture reading to percentage

  // Display soil moisture percentage on the LCD
  lcd.setCursor(0, 1);
  lcd.print("Moisture: ");
  lcd.print(moisturePercent);
  lcd.print("%");

  // Control relay based on soil moisture level
  lcd.setCursor(12, 0); // Set cursor position for motor status
  if (moisturePercent < 30) { // Turn on the motor if moisture is below 30%
    digitalWrite(relayPin, LOW); // Relay LOW turns on the pump (active-low)
    lcd.print("Pump ON ");
  } else {
    digitalWrite(relayPin, HIGH); // Relay HIGH turns off the pump
    lcd.print("Pump OFF");
  }

  delay(2000); // Wait for 2 seconds before next reading
}
