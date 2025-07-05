#include <ESP8266WiFi.h>
#include <Servo.h>

// =======================
// Define Pin Connections
// =======================
#define TRIG1 D1         // Ultrasonic sensor 1 - Trigger
#define ECHO1 D2         // Ultrasonic sensor 1 - Echo
#define TRIG2 D3         // Ultrasonic sensor 2 - Trigger
#define ECHO2 D4         // Ultrasonic sensor 2 - Echo
#define PIR_PIN D5       // PIR sensor for motion detection
#define SERVO_PIN D6     // Servo motor pin (connected to PIR housing)
#define EMITTER_PIN D7   // Ultrasonic sound emitter to scare animal
#define TILT_PIN D8      // Tilt sensor pin (LOW = tilted/fallen)

Servo pirServo;

// =======================
// Animal Detection Control
// =======================
int animalDetectedCount = 0;
const int detectionThreshold = 3;  // No. of PIR hits during servo scan before STOP alert

// =======================
// Servo Control Variables
// =======================
bool clockwise = true;
unsigned long servoStartTime = 0;
const unsigned long rotationTime = 2500; // Time in ms to simulate one rotation
bool isRotating = false;

void setup() {
  Serial.begin(115200);

  // Set pin modes for sensors and actuators
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(EMITTER_PIN, OUTPUT);
  pinMode(TILT_PIN, INPUT_PULLUP);  // LOW = tilted

  pirServo.attach(SERVO_PIN);       // Attach servo to its control pin

  digitalWrite(EMITTER_PIN, LOW);   // Make sure emitter is OFF initially
}

// ==========
// Ultrasonic distance reader
// Returns distance in centimeters
// ==========
long readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) / 58;
}

// ==========
// Servo control simulating continuous 360° slow rotation
// Rotates slowly in one direction for a set duration
// Then switches direction
// ==========
void rotateServoContinuously() {
  if (!isRotating) {
    servoStartTime = millis();
    isRotating = true;

    if (clockwise) {
      pirServo.write(110); // Slow clockwise
      Serial.println("Servo rotating clockwise...");
    } else {
      pirServo.write(70);  // Slow counter-clockwise
      Serial.println("Servo rotating counter-clockwise...");
    }
  }

  if (millis() - servoStartTime >= rotationTime) {
    pirServo.write(90);  // Stop servo
    clockwise = !clockwise; // Change direction
    isRotating = false;
    delay(500); // Pause between rotations
  }
}

// ==========
// Loop: Main Logic
// ==========
void loop() {
  // Sensor readings
  long distance1 = readUltrasonic(TRIG1, ECHO1);  // Distance to next post
  long distance2 = readUltrasonic(TRIG2, ECHO2);  // Distance to previous post
  bool pirDetected = digitalRead(PIR_PIN);        // Motion detection
  bool tiltFallen = digitalRead(TILT_PIN) == LOW; // Post tilted or fallen

  // Print all sensor readings
  Serial.println("Distances (cm): " + String(distance1) + ", " + String(distance2));
  Serial.println("PIR Detected: " + String(pirDetected));
  Serial.println("Tilt Fallen: " + String(tiltFallen));

  // =======================
  // TILT SENSOR: Check if the post is fallen
  // =======================
  if (tiltFallen) {
    Serial.println("⚠️ Post is fallen! Send alert.");
    // sendAlert("Post Tilted");
  }

  // =======================
  // ANIMAL NEARBY: PIR detects motion, but ultrasonic does not (no close object)
  // =======================
  if ((distance1 > 300 || distance2 > 300) && pirDetected) {
    Serial.println("🚨 Animal detected nearby. Slow down train.");
    // sendAlert("Animal nearby. Slow train.");
  }

  // =======================
  // ANIMAL ON TRACK: PIR + object detected close by
  // =======================
  else if ((distance1 < 100 || distance2 < 100) && pirDetected) {
    Serial.println("🚨 Animal on track. Activating sound emitter.");
    digitalWrite(EMITTER_PIN, HIGH);  // Turn ON emitter

    animalDetectedCount = 0;

    // Monitor with several servo sweeps to check if animal leaves
    for (int i = 0; i < detectionThreshold; i++) {
      rotateServoContinuously();  // Sweep while checking for PIR
      delay(rotationTime);        // Wait for one rotation
      if (digitalRead(PIR_PIN)) {
        animalDetectedCount++;
        Serial.println("Still detecting motion...");
      }
    }

    // If animal is still there after sweeps
    if (animalDetectedCount >= detectionThreshold) {
      Serial.println("🛑 Animal hasn't left. Send EMERGENCY STOP alert!");
      // sendAlert("STOP train! Animal blocking track.");
    } else {
      Serial.println("✅ Animal has left. Deactivating emitter.");
    }

    digitalWrite(EMITTER_PIN, LOW);  // Turn OFF emitter
  }

  // =======================
  // Normal rotation (no alerts)
  // =======================
  rotateServoContinuously();

  delay(500); // General loop delay for readability
}
