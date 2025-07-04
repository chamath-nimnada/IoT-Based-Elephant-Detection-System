// Tilt Sensor
#define TILT_SENSOR A0  // Analog pin for tilt sensor

// Ultrasonic Sensor 1
const int trigPin1 = D1; // GPIO5
const int echoPin1 = D2; // GPIO4

// Ultrasonic Sensor 2
const int trigPin2 = D5; // GPIO14
const int echoPin2 = D6; // GPIO12

void setup() {
  Serial.begin(9600);

  // Tilt sensor pin setup
  pinMode(TILT_SENSOR, INPUT);

  // Ultrasonic sensor pin setup
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

void loop() {
  // Check and display tilt status
  checkTilt();

  // Read distances from both ultrasonic sensors
  float distance1 = getDistance(trigPin1, echoPin1);
  float distance2 = getDistance(trigPin2, echoPin2);

  // Display distances
  Serial.print("Ultrasonic 1 Distance: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Ultrasonic 2 Distance: ");
  Serial.print(distance2);
  Serial.println(" cm");

  Serial.println("-------------------------");
  delay(500); // Small delay before next reading
}

// 📦 Function to measure distance from any ultrasonic sensor
float getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 30000); // Timeout at 30ms
  float distance = duration * 0.0343 / 2;
  return distance;
}

// 🧭 Function to check tilt sensor status
void checkTilt() {
  int tiltValue = analogRead(TILT_SENSOR);

  Serial.print("Tilt Sensor Reading: ");
  Serial.println(tiltValue);

  if (tiltValue > 200) { // Adjust this threshold for your sensor
    Serial.println("ALERT: Post is tilted or fallen. Inspect immediately!");
  }
}