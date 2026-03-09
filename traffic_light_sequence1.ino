const int red = 8;
const int yellow = 9;
const int green = 10;
const int btn = 2;

const int crsRed = 11;
const int crsGreen = 12;

const int reqLED = 7;

const int buzzPin = 6;

const int trig = 4;
const int echo = 5;

bool btnPress = false;


long getDist(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  long time = pulseIn(echo, HIGH, 30000);
  long distance = time * 0.0343 / 2;
  
  return distance;
}

void setup() {
pinMode(red, OUTPUT);
pinMode(yellow, OUTPUT);
pinMode(green, OUTPUT);

pinMode(crsRed, OUTPUT);
pinMode(crsGreen, OUTPUT);

pinMode(reqLED, OUTPUT);

pinMode(btn, INPUT);
  
pinMode(buzzPin, OUTPUT);
  
pinMode(trig, OUTPUT);
pinMode(echo, INPUT);

digitalWrite(green, HIGH);
digitalWrite(crsRed, HIGH);
digitalWrite(reqLED, LOW);
}

void loop() {

  if (digitalRead(btn) == HIGH){
    btnPress = true;
    digitalWrite(reqLED, HIGH);
  }
  if (!btnPress){
    return;
  }

  delay(3000);

  btnPress = false;

// Yellow
  digitalWrite(green, LOW);
  digitalWrite(yellow, HIGH);
  delay (2000);

// Red
  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH);
  delay (3000);

// Crossing Green
  digitalWrite(crsRed, LOW);
  digitalWrite(crsGreen, HIGH);
  digitalWrite(reqLED, LOW);
  
  unsigned long walkStart = millis();
  const unsigned long minWalkTime = 5000;
  const unsigned long maxWalkTime = 15000;
  const int distDetect = 80;
  
  while(true){
    unsigned long timeStamp = millis();
    long sensorDist = getDist();
    
    bool pedPresent = (sensorDist > 0 && sensorDist < distDetect);
	 
tone(buzzPin, 2500, 100);
delay(100);
    
    if (!pedPresent && (timeStamp - walkStart) > minWalkTime) {
      break;
    }
    if ((timeStamp - walkStart) > maxWalkTime) {
      break;
    }
    delay(100);
  }
    
// Crossing Red
	noTone(buzzPin);
  digitalWrite(crsRed, HIGH);
  digitalWrite(crsGreen, LOW);

//Hold Traffic Red
  delay(3000);

  digitalWrite(yellow, HIGH);
  delay(3000);

  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, HIGH);
}