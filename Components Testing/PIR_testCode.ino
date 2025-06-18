#define PIR_PIN D5

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
}

void loop() {
  int motion = digitalRead(PIR_PIN);
  if (motion == HIGH) {
    Serial.println("🚨 Motion Detected!");
  } else {
    Serial.println("No motion.");
  }
  delay(1000);
}
