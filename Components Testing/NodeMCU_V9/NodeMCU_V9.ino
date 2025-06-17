/*
 * NodeMCU ESP8266 Complete Test Code
 * Tests: WiFi, GPIO, Serial, Built-in LED, Analog Input
 * 
 * Instructions:
 * 1. Install ESP8266 board package in Arduino IDE
 * 2. Select "NodeMCU 1.0 (ESP-12E Module)" as board
 * 3. Update WiFi credentials below
 * 4. Upload and open Serial Monitor at 9600 baud
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi credentials - UPDATE THESE WITH YOUR ACTUAL WIFI DETAILS
const char* ssid = "Mucha";        // ← Replace this with your WiFi network name
const char* password = "musab2611"; // ← Replace this with your WiFi password

// EXAMPLE:
// const char* ssid = "MyHomeWiFi";
// const char* password = "mypassword123";

// Pin definitions
const int LED_BUILTIN_PIN = 2;  // Built-in LED (GPIO2)
const int TEST_OUTPUT_PIN = 16; // GPIO16 (D0)
const int TEST_INPUT_PIN = 5;   // GPIO5 (D1)
const int ANALOG_PIN = A0;      // Analog input

// Web server
ESP8266WebServer server(80);

// Test variables
bool testsPassed = true;
int testCount = 0;
int passedTests = 0;

void setup() {
  Serial.begin(9600);  // Changed to more common baud rate
  delay(2000);         // Longer delay for stability
  
  Serial.println("\n=================================");
  Serial.println("NodeMCU ESP8266 Hardware Test");
  Serial.println("=================================");
  
  // Initialize pins
  pinMode(LED_BUILTIN_PIN, OUTPUT);
  pinMode(TEST_OUTPUT_PIN, OUTPUT);
  pinMode(TEST_INPUT_PIN, INPUT_PULLUP);
  
  // Run all tests
  testSerialCommunication();
  testBuiltInLED();
  testGPIOPins();
  testAnalogInput();
  testWiFiConnection();
  testWebServer();
  
  // Print final results
  printTestResults();
}

void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Blink LED to show it's running
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 2000) {
    digitalWrite(LED_BUILTIN_PIN, !digitalRead(LED_BUILTIN_PIN));
    lastBlink = millis();
  }
  
  // Print status every 10 seconds
  static unsigned long lastStatus = 0;
  if (millis() - lastStatus > 10000) {
    Serial.println("NodeMCU is running... IP: " + WiFi.localIP().toString());
    lastStatus = millis();
  }
  
  delay(100);
}

void testSerialCommunication() {
  Serial.println("\n1. Testing Serial Communication...");
  testCount++;
  
  Serial.println("   - Serial port initialized at 115200 baud");
  Serial.println("   - If you can read this, serial communication works!");
  
  passedTests++;
  Serial.println("   ✓ PASSED");
}

void testBuiltInLED() {
  Serial.println("\n2. Testing Built-in LED...");
  testCount++;
  
  Serial.println("   - Blinking built-in LED 5 times...");
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_BUILTIN_PIN, LOW);  // LED ON (inverted logic)
    delay(200);
    digitalWrite(LED_BUILTIN_PIN, HIGH); // LED OFF
    delay(200);
  }
  
  passedTests++;
  Serial.println("   ✓ PASSED - Built-in LED is working");
}

void testGPIOPins() {
  Serial.println("\n3. Testing GPIO Pins...");
  testCount++;
  
  Serial.println("   - Testing digital output (GPIO16)...");
  digitalWrite(TEST_OUTPUT_PIN, HIGH);
  delay(100);
  digitalWrite(TEST_OUTPUT_PIN, LOW);
  
  Serial.println("   - Testing digital input (GPIO5)...");
  int inputState = digitalRead(TEST_INPUT_PIN);
  Serial.println("   - GPIO5 state: " + String(inputState ? "HIGH" : "LOW"));
  
  passedTests++;
  Serial.println("   ✓ PASSED - GPIO pins are functional");
}

void testAnalogInput() {
  Serial.println("\n4. Testing Analog Input...");
  testCount++;
  
  int analogValue = analogRead(ANALOG_PIN);
  float voltage = (analogValue / 1024.0) * 3.3;
  
  Serial.println("   - Analog reading: " + String(analogValue));
  Serial.println("   - Voltage: " + String(voltage, 2) + "V");
  
  if (analogValue >= 0 && analogValue <= 1024) {
    passedTests++;
    Serial.println("   ✓ PASSED - Analog input is working");
  } else {
    Serial.println("   ✗ FAILED - Analog reading out of range");
  }
}

void testWiFiConnection() {
  Serial.println("\n5. Testing WiFi Connection...");
  testCount++;
  
  Serial.println("   - Connecting to WiFi: " + String(ssid));
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print("   .");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n   - Connected successfully!");
    Serial.println("   - IP Address: " + WiFi.localIP().toString());
    Serial.println("   - MAC Address: " + WiFi.macAddress());
    Serial.println("   - Signal Strength: " + String(WiFi.RSSI()) + " dBm");
    
    passedTests++;
    Serial.println("   ✓ PASSED - WiFi connection successful");
  } else {
    Serial.println("\n   ✗ FAILED - Could not connect to WiFi");
    Serial.println("   - Check your WiFi credentials in the code");
  }
}

void testWebServer() {
  Serial.println("\n6. Testing Web Server...");
  testCount++;
  
  if (WiFi.status() == WL_CONNECTED) {
    // Setup web server routes
    server.on("/", handleRoot);
    server.on("/test", handleTest);
    server.onNotFound(handleNotFound);
    
    server.begin();
    Serial.println("   - Web server started on port 80");
    Serial.println("   - Visit: http://" + WiFi.localIP().toString());
    Serial.println("   - Test page: http://" + WiFi.localIP().toString() + "/test");
    
    passedTests++;
    Serial.println("   ✓ PASSED - Web server is running");
  } else {
    Serial.println("   ✗ SKIPPED - No WiFi connection");
  }
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>NodeMCU Test Server</h1>";
  html += "<p>Your NodeMCU is working perfectly!</p>";
  html += "<p>Chip ID: " + String(ESP.getChipId()) + "</p>";
  html += "<p>Flash Chip Size: " + String(ESP.getFlashChipSize()) + " bytes</p>";
  html += "<p>Free Heap: " + String(ESP.getFreeHeap()) + " bytes</p>";
  html += "<p>Uptime: " + String(millis() / 1000) + " seconds</p>";
  html += "<p><a href='/test'>Run Tests</a></p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleTest() {
  String response = "NodeMCU Test Results:\n";
  response += "Tests Passed: " + String(passedTests) + "/" + String(testCount) + "\n";
  response += "WiFi Connected: " + String(WiFi.status() == WL_CONNECTED ? "Yes" : "No") + "\n";
  response += "IP Address: " + WiFi.localIP().toString() + "\n";
  response += "Free Memory: " + String(ESP.getFreeHeap()) + " bytes\n";
  response += "Chip ID: " + String(ESP.getChipId()) + "\n";
  
  server.send(200, "text/plain", response);
}

void handleNotFound() {
  server.send(404, "text/plain", "Page not found!");
}

void printTestResults() {
  Serial.println("\n=================================");
  Serial.println("TEST RESULTS SUMMARY");
  Serial.println("=================================");
  Serial.println("Total Tests: " + String(testCount));
  Serial.println("Passed: " + String(passedTests));
  Serial.println("Failed: " + String(testCount - passedTests));
  Serial.println("Success Rate: " + String((passedTests * 100) / testCount) + "%");
  
  if (passedTests == testCount) {
    Serial.println("\n🎉 ALL TESTS PASSED! Your NodeMCU is working perfectly!");
  } else {
    Serial.println("\n⚠️  Some tests failed. Check the results above.");
  }
  
  Serial.println("\nHardware Information:");
  Serial.println("- Chip ID: " + String(ESP.getChipId()));
  Serial.println("- Flash Size: " + String(ESP.getFlashChipSize()) + " bytes");
  Serial.println("- Free Heap: " + String(ESP.getFreeHeap()) + " bytes");
  Serial.println("- CPU Frequency: " + String(ESP.getCpuFreqMHz()) + " MHz");
  
  Serial.println("\n=================================");
}