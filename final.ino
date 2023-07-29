//Designed, Developed, and Tested By Kush Gandhi
// Libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD Pins
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Soil Moisture Sensor Pins
const byte soilPins[] = {A0, A1, A2, A3};

// Pump Pins
const byte pumpPins[] = {3, 5, 6, 9};

// Power Switch
const byte powerPin = 8;

// LED Pins
const byte powerLED = 7;

// Piezo Pin
const byte piezo = 4;

// Constants
const int moistureThreshold = 20; // Moisture threshold for watering (%)


void setup()
{
  // Initializing Pins for Moisture Sensors
  for (byte i = 0; i < 4; i++)
    pinMode(soilPins[i], INPUT);

  // Initializing Pins for Pumps
  for (byte i = 0; i < 4; i++)
    pinMode(pumpPins[i], OUTPUT);

  // Initializing LCD
  lcd.init();
  lcd.backlight();

  // Initializing Piezo
  pinMode(piezo, OUTPUT);

  // Initializing Power Switch Pin
  pinMode(powerPin, INPUT);

  // Initializing Power LED
  pinMode(powerLED, OUTPUT);
}

void loop()
{
  // Checking if Power Switch is ON
  if(digitalRead(powerPin) == LOW)
  {
    // Turning on the power LED
    digitalWrite(powerLED, HIGH);

    irrigate();
  }
  else
  {
    // System Off
    // Turn off pumps
    for (byte i = 0; i < 4; i++)
      digitalWrite(pumpPins[i], LOW);

    // Turn off power LED
    digitalWrite(powerLED, LOW);

    // Clear the LCD
    lcd.clear();
    
    // Piezo Off
    digitalWrite(piezo, LOW);
  }
}

void irrigate()
{
  lcd.setCursor(0,0);
  lcd.print(" P1  P2  P3  P4");

  for (byte i = 0; i < 4; i++)
  {
    // Read sensor value
    int moisture = analogRead(soilPins[i]);
    
    // Calculate percentage
    int moisturePercent = map(moisture, 220, 500, 100, 0);

    // Print moisture value on LCD
    lcd.setCursor(i * 4,1); // Set cursor to the correct position for each plant
    lcd.print(moisturePercent);
    lcd.print("%");

    // Check if plant needs watering
    while (moisturePercent < moistureThreshold)
    {
      // Activate pump
      digitalWrite(pumpPins[i], LOW);

      // Piezo On
      digitalWrite(piezo, HIGH);

      // Read sensor value
      moisture = analogRead(soilPins[i]);
      
      // Calculate percentage
      moisturePercent = map(moisture, 220, 500, 100, 0);

      // Print moisture value on LCD
      lcd.setCursor(i * 4, 1);
      lcd.print(moisturePercent);
      lcd.print("%");

      delay(1000);
    }

    // Deactivate pump
    digitalWrite(pumpPins[i], HIGH);
    
    // Piezo Off
    digitalWrite(piezo, LOW);

    // Clear previous moisture value for this plant from the LCD
    lcd.setCursor(i * 4, 1);
    lcd.print("    ");

    // Wait for a bit before next reading
    delay(1000);
  }
}



