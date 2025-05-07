#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
const int mq2Pin = A0;
const int flameSensorPin = A1;
const int motionSensorPin = 4;
const int buzzerPin = 8;
const int ledPin = 9;
// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Threshold values
const int MQ2Threshold = 230; // Adjust based on testing
const int flameThreshold = 300; // Adjust based on testing
void setup() {
  // Initialize serial communications
  Serial.begin(9600);
  // Set pin modes
  pinMode(mq2Pin, INPUT);
  pinMode(flameSensorPin, INPUT);
  pinMode(motionSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read sensor values
  int mq2Value = analogRead(mq2Pin);
  int flameValue = analogRead(flameSensorPin);
  int motionValue = digitalRead(motionSensorPin);

  // Check each sensor
  if (mq2Value > MQ2Threshold) {
    lcdMessage("Gas Leak Detected!");
    activateAlarm();
    Serial.println('G');
    delay(1200);//You can remove the delay if it affects the reading
  }

  if (flameValue < flameThreshold) {
    lcdMessage("Fire Detected!");
    activateAlarm();
    Serial.println('F');
    delay(1200);//You can remove the delay if it affects the reading
  }
 if (motionValue == HIGH) {
    lcdMessage("Motion Detected!");
    activateAlarm();
    Serial.println('M');
    delay(1200); //You can remove the delay if it affects the reading
  }

  // Small delay before the next loop
  delay(1000);
}

// Display message on LCD
void lcdMessage(const char* message) {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print(message);
  delay(900);
  lcd.clear();
}

// Helper function to activate buzzer and LED
void activateAlarm() {
  digitalWrite(buzzerPin, HIGH);
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
   digitalWrite(buzzerPin, LOW);
 }