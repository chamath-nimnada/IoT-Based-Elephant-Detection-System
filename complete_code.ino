Updated code "#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>

// Wi-Fi credentials
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"

// Firebase credentials
#define FIREBASE_HOST "iot-animal-detection-system-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "your_firebase_database_secret"

// Firebase object
FirebaseData firebaseData;

Servo servoMotor;

// Define pins
#define TRIG1 D1
#define ECHO1 D2
#define TRIG2 D5
#define ECHO2 D6
#define PIR_FRONT D7
#define PIR_BACK D8
#define SERVO_PIN D4
#define EMITTER_PIN D3
#define TILT_SENSOR A0

// Variables
unsigned long lastFrontMotionTime = 0;
bool pirFrontActive = true;
int animalStillDetectedCount = 0;
const int detectionThreshold = 3;
const unsigned long NO_MOTION_TIMEOUT = 60000; // 1 minute

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  // Pin setup
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
  digitalWrite(EMITTER_PIN, LOW);
}

long readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH, 30000) / 58;
}

float getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 30000);
  return duration * 0.0343 / 2;
}

void sendToFirebase(float d1, float d2, int tilt, bool pirFront, bool pirBack) {
  Firebase.setFloat(firebaseData, "/Ultrasonic/Distance1", d1);
  Firebase.setFloat(firebaseData, "/Ultrasonic/Distance2", d2);
  Firebase.setInt(firebaseData, "/TiltSensor", tilt);
  Firebase.setBool(firebaseData, "/PIR/Front", pirFront);
  Firebase.setBool(firebaseData, "/PIR/Back", pirBack);
}

void checkTilt(int tiltValue) {
  if (tiltValue > 300) {
    Serial.println("ALERT: Post is tilted or fallen.");
  }
}

void checkPIRBack(bool pirBack) {
  if (pirBack) {
    Serial.println("Motion behind! Animal detected.");
  }
}

void checkTrackIntrusion(long d1, long d2, bool frontPIR) {
  if (d1 < 25 || d2 < 25) {
    Serial.println("Obstacle within 25cm.");
  }

  if (frontPIR) {
    Serial.println("ALERT: STOP TRAIN. Animal detected.");
    digitalWrite(EMITTER_PIN, HIGH);
    animalStillDetectedCount = 0;

    for (int i = 0; i < detectionThreshold; i++) {
      rotatePIRwithSensors();
      if (digitalRead(PIR_FRONT) == HIGH) {
        animalStillDetectedCount++;
      }
    }

    if (animalStillDetectedCount >= detectionThreshold) {
      Serial.println("EMERGENCY: Animal not leaving.");
    } else {
      Serial.println("Animal moved away.");
    }

    digitalWrite(EMITTER_PIN, LOW);
  }
}

void readUltrasonicSensors() {
  float distance1 = getDistance(TRIG1, ECHO1);
  float distance2 = getDistance(TRIG2, ECHO2);
  int tiltValue = analogRead(TILT_SENSOR);
  bool pirFront = digitalRead(PIR_FRONT);
  bool pirBack = digitalRead(PIR_BACK);

  Serial.print("Distance1: ");
  Serial.print(distance1);
  Serial.print(" | Distance2: ");
  Serial.print(distance2);
  Serial.print(" | Tilt: ");
  Serial.print(tiltValue);
  Serial.print(" | PIR Front: ");
  Serial.print(pirFront);
  Serial.print(" | PIR Back: ");
  Serial.println(pirBack);

  // Send to Firebase
  sendToFirebase(distance1, distance2, tiltValue, pirFront, pirBack);

  checkTilt(tiltValue);
  checkPIRBack(pirBack);
  checkTrackIntrusion(distance1, distance2, pirFront);

  delay(1000);
}

void rotatePIRwithSensors() {
  for (int angle = 0; angle <= 180; angle += 5) {
    servoMotor.write(angle);
    delay(50);
    readUltrasonicSensors();
  }
  for (int angle = 180; angle >= 0; angle -= 5) {
    servoMotor.write(angle);
    delay(50);
    readUltrasonicSensors();
  }
}

void loop() {
  rotatePIRwithSensors();
  delay(2000);
}
"