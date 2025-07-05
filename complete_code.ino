#include <Servo.h>

// Define pins
#define TRIG1 D1
#define ECHO1 D2
#define TRIG2 D5
#define ECHO2 D6
#define PIR_FRONT D7
#define PIR_BACK D8
#define SERVO_PIN D4
#define EMITTER_PIN D3
#define TILT_SENSOR A0  // Analog input for tilt

Servo servoMotor;

// Time tracking
unsigned long lastFrontMotionTime = 0;
bool pirFrontActive = true;
int animalStillDetectedCount = 0;
const int detectionThreshold = 3;
const unsigned long NO_MOTION_TIMEOUT = 60000; // 1 minute

void setup() {
  Serial.begin(9600);

  // Setup pins
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(PIR_FRONT, INPUT);
  pinMode(PIR_BACK, INPUT);
  pinMode(EMITTER_PIN, OUTPUT);
  pinMode(TILT_SENSOR, INPUT);

  servoMotor.attach(SERVO_PIN);
   servoMotor.write(0); 
         // Set initial position to 0 degrees
  digitalWrite(EMITTER_PIN, LOW); // Initially off

  Serial.println("System Initialized.");
  delay(5000);   
}

// Read distance from ultrasonic sensor
long readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) / 58; // Convert to cm
}

// Check if post is fallen using tilt sensor
void checkTilt() {
  int tiltValue = analogRead(TILT_SENSOR);
  if (tiltValue > 300) { // Adjust based on your sensor
    Serial.println("ALERT: Post is tilted or fallen. Inspect immediately!");
  }
}

// Rotate servo for PIR scanning
void rotatePIR() {
  for (int angle = 0; angle <= 180; angle += 5) {
    servoMotor.write(angle);
    delay(50);
  }
  for (int angle = 180; angle >= 0; angle -= 5) {
    servoMotor.write(angle);
    delay(50);
  }
}

// Check for animal near track (detected behind post)
void checkPIRBack() {
  if (digitalRead(PIR_BACK) == HIGH) {
    Serial.println("Motion detected behind! Animal nearby. Slow down train.");
    delay(2000);
  }
}

// Check for animals on track using ultrasonic + PIR front
void checkTrackIntrusion(long d1, long d2) {
  bool frontPIR = digitalRead(PIR_FRONT);

  if (d1 < 25 || d2 < 25) {
    Serial.println("Obstacle detected within 25cm: An animal came into the track. Slow down the train !");
    delay(2000);
  }

  if (frontPIR) {
    Serial.println("ALERT: STOP TRAIN! Animal is on the track.");
    delay(2000);
    digitalWrite(EMITTER_PIN, HIGH); // Turn on ultrasonic sound emitter
    animalStillDetectedCount = 0;

    for (int i = 0; i < detectionThreshold; i++) {
      rotatePIR();
      if (digitalRead(PIR_FRONT) == HIGH) {
        animalStillDetectedCount++;
        delay(500);
      }
    }

    if (animalStillDetectedCount >= detectionThreshold) {
      Serial.println("EMERGENCY: Animal not leaving. Immediate STOP required!");
      delay(2000);
    } else {
      Serial.println("Animal moved away. Deactivating emitter.");
      delay(2000);
    }

    digitalWrite(EMITTER_PIN, LOW);
  }
}

// Monitor PIR front sensor for motion disappearance


void readUltrasonicSensors() {
float distance1 = getDistance(TRIG1, ECHO1);
float distance2 = getDistance(TRIG2, ECHO2);


  Serial.print("Ultrasonic 1 Distance: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Ultrasonic 2 Distance: ");
  Serial.print(distance2);
  Serial.println(" cm");

  Serial.println("-------------------------");
  delay(2000);
}

// 📏 Function to measure distance from an ultrasonic sensor
float getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 30000); // Timeout: 30 ms
  float distance = duration * 0.0343 / 2;     // Convert to cm
  return distance;
}

void loop() {
  // Read ultrasonic distances
  long distance1 = readUltrasonic(TRIG1, ECHO1);
  long distance2 = readUltrasonic(TRIG2, ECHO2);

  // Call all core functions
  readUltrasonicSensors();
   checkTilt();
   checkPIRBack();
   checkTrackIntrusion(distance1, distance2);

  
}