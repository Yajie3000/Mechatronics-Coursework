#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// --- Pin Definitions ---
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int SERVO_PIN = 6;
const int POT_PIN = A0;

// --- Objects ---
Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// --- Variables ---
long duration;
int distanceCm;
int potValue;
int detectionRange; // The range set by potentiometer (10-335cm)
bool carDetected = false; // Track current state
bool lastCarDetected = false; // Track previous state for LCD updates

void setup() {
  Serial.begin(9600);
  Serial.println("Car Park System Starting...");

  // Initialize sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Attach the servo
  myServo.attach(SERVO_PIN);
  myServo.write(0); // Start with barrier closed

  // Initialize the LCD
  delay(500);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Show startup message
  lcd.setCursor(0, 0);
  lcd.print("Car Park System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  
  lcd.clear();
  updateDisplay(false, detectionRange); // Show initial state
  
  Serial.println("Setup Complete!");
}

void loop() {
  // 1. Read the potentiometer to set detection range (10-335cm)
  potValue = analogRead(POT_PIN);
  detectionRange = map(potValue, 0, 1023, 10, 335);

  // 2. Measure distance from ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate distance (with timeout check)
  if (duration == 0) {
    distanceCm = 999; // Set to large value if no reading
  } else {
    distanceCm = duration * 0.0343 / 2;
  }

  // 3. Check if car is detected within the set range
  if (distanceCm > 0 && distanceCm < detectionRange) {
    // Car detected within the potentiometer-set range
    carDetected = true;
    myServo.write(90); // Raise barrier
    Serial.print("CAR DETECTED at ");
    Serial.print(distanceCm);
    Serial.print("cm (Range: ");
    Serial.print(detectionRange);
    Serial.println("cm) - Barrier UP");
  } else {
    // No car in detection range
    carDetected = false;
    myServo.write(0); // Lower barrier
    Serial.print("NO CAR - Distance: ");
    Serial.print(distanceCm);
    Serial.print("cm (Range: ");
    Serial.print(detectionRange);
    Serial.println("cm) - Barrier DOWN");
  }

  // 4. Update LCD only if car detection state 
  if (carDetected != lastCarDetected) {
    updateDisplay(carDetected, detectionRange);
    lastCarDetected = carDetected;
  }

  delay(100); // Small delay for stability
}

// Function to update the LCD display with status messages
void updateDisplay(bool carPresent, int range) {
  lcd.clear();
  
  if (carPresent) {
    // Car detected message
    lcd.setCursor(0, 0);
    lcd.print("CAR DETECTED!   ");
    lcd.setCursor(0, 1);
    lcd.print("Barrier UP      ");
  } else {
    // No car message
    lcd.setCursor(0, 0);
    lcd.print("NO CAR          ");
    lcd.setCursor(0, 1);
    lcd.print("Barrier DOWN    ");
  }
  
  
} 