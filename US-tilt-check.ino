// Tilt Sensor
#define TILT_PIN D7 

// Ultrasonic Sensor - Left
#define TRIG_LEFT D1
#define ECHO_LEFT D2

// Ultrasonic Sensor - Right
#define TRIG_RIGHT D3
#define ECHO_RIGHT D4

// To track tilt state
bool wasTilted = false; 


void setup() {
  // Tilt Sensor pin
  pinMode(TILT_PIN, INPUT);

  // Left Ultrasonic Sensor pins
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);

  // Right Ultrasonic Sensor pins
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  // Start Serial Monitor
  Serial.begin(115200);
}


void loop() {
  // to Tilt Detection
  int tiltValue = digitalRead(TILT_PIN);

  if (tiltValue == HIGH && !wasTilted) {
    Serial.println("⚠ ALERT: Tower Tilt Detected!");
    wasTilted = true;
  } else if (tiltValue == LOW && wasTilted) {
    Serial.println("✅ Tower is Stable.");
    wasTilted = false;
  }

  // for Ultrasonic Sensors
  float distanceLeft = measureDistance(TRIG_LEFT, ECHO_LEFT);
  float distanceRight = measureDistance(TRIG_RIGHT, ECHO_RIGHT);

  Serial.print("Left Distance: ");
  Serial.print(distanceLeft);
  Serial.print(" cm\t");

  Serial.print("Right Distance: ");
  Serial.print(distanceRight);
  Serial.println(" cm");

  // Wildlife alert if any object is too close
  if (distanceLeft < 100 || distanceRight < 100) {
    Serial.println("🦌 Wildlife Detected Near Tower!");
  }

  delay(5000); // Wait before next loop
}

// function to measure the distance
// Sends a pulse and reads echo time to calculate distance in cm
float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH); // Time in microseconds
  float distance = duration * 0.034 / 2;  // Convert to cm

  return distance;
}