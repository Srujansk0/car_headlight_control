int sensorPin = 34;  // Define the pin where TEMT6000 is connected (Analog pin)

void setup() {
  // Start Serial communication
  Serial.begin(115200);

  // Initialize the sensor pin as an input
  pinMode(sensorPin, INPUT);

  Serial.println("TEMT6000 Ambient Light Sensor Initialized");
}

void loop() {
  // Read the analog value from the TEMT6000
  int sensorValue = analogRead(sensorPin);

  // Convert the analog value to a voltage (ESP32 has a 12-bit ADC, so the range is 0-4095)
  float voltage = sensorValue * (3.3 / 4095.0);  // 3.3V reference voltage

  // Display the sensor value and corresponding voltage on the Serial Monitor
  Serial.print("Analog Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);  // Display voltage with 2 decimal places
  Serial.println(" V");

  // Delay for readability
  delay(1000);
}
