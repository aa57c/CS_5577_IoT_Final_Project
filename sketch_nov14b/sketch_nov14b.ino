#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// To download LiquidCrystal, follow the instructions in this youtube video: https://www.youtube.com/watch?v=j17ewTL7UHo&t=134s&ab_channel=ElectronicsSimplified

// Set the LCD address
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define buzzer and button pins
const int buttonPin = 5;  // GPIO 5 (D5)
const int buzzerPin = 4;  // GPIO 4 (D4)

// Simulated environment variables
float simulatedTemperature = 25.5; // Initial temperature (Celsius)
float simulatedHumidity = 60.0;    // Initial humidity (%)
float simulatedGasLevel = 50.0;    // Initial methane level (ppm)
int loopCounter = 0;               // Counter for test scenarios
bool gasAlert = false;             // Gas alert flag
bool temperatureAlert = false;     // Temperature alert flag

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Set buzzer and button pins
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  loopCounter++; // Increment loop counter

  // Force high temperature and methane levels for testing
  if (loopCounter == 10) {
    simulatedTemperature = 37.0; // Simulate high temperature
    simulatedGasLevel = 400.0;   // Simulate high methane level
  }

  // Simulate dynamic changes
  simulateEnvironmentalChanges();

  // Check for alerts
  temperatureAlert = (simulatedTemperature > 35.0);
  gasAlert = (simulatedGasLevel > 300.0);

  if (temperatureAlert || gasAlert) {
    handleAlerts();
  } else {
    // Display normal readings if no alerts
    displayTemperatureAndHumidity();
    delay(3000); // Normal display for 3 seconds
    displayGasLevel();
    delay(3000); // Gas display for 3 seconds
  }

  delay(1000); // Short delay before the next iteration
}

// Simulate environmental changes
void simulateEnvironmentalChanges() {
  simulatedTemperature += random(-2, 3) * 0.5; // -1.0 to +1.0
  simulatedHumidity += random(-3, 4) * 0.5;    // -1.5 to +1.5
  simulatedGasLevel += random(-50, 51);        // -50 to +50 ppm

  // Clamp values
  simulatedTemperature = constrain(simulatedTemperature, 15.0, 50.0);
  simulatedHumidity = constrain(simulatedHumidity, 20.0, 90.0);
  simulatedGasLevel = constrain(simulatedGasLevel, 0.0, 1000.0);
}

// Display temperature and humidity
void displayTemperatureAndHumidity() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(simulatedTemperature);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(simulatedHumidity);
  lcd.print("%");
}

// Display methane gas level
void displayGasLevel() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Methane: ");
  lcd.print(simulatedGasLevel);
  lcd.print(" ppm");
}

// Handle alerts
void handleAlerts() {
  static unsigned long lastBuzzerTime = 0; // Track the last time the buzzer sounded
  static unsigned long lastDisplayTime = 0; // Track the last display update
  unsigned long currentTime = millis();

  // Sound buzzer every 5 seconds if any alert is active
  if (currentTime - lastBuzzerTime >= 5000) {
    digitalWrite(buzzerPin, HIGH);
    delay(500); // Buzzer on for 0.5 seconds
    digitalWrite(buzzerPin, LOW);
    lastBuzzerTime = currentTime;
  }

  // If both alerts are active, alternate between them on the display
  if (temperatureAlert && gasAlert) {
    if (currentTime - lastDisplayTime >= 3000) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp Too High!");
      lcd.setCursor(0, 1);
      lcd.print("Temp: ");
      lcd.print(simulatedTemperature);
      lcd.print("C");
      lastDisplayTime = currentTime;
    }
  } else {
    // If only one alert is active, display that alert
    if (temperatureAlert) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp Too High!");
      lcd.setCursor(0, 1);
      lcd.print("Temp: ");
      lcd.print(simulatedTemperature);
      lcd.print("C");
    } else if (gasAlert) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Gas Alert!");
      lcd.setCursor(0, 1);
      lcd.print("Methane: ");
      lcd.print(simulatedGasLevel);
      lcd.print(" ppm");
    }
  }
}
