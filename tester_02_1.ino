#include <Wire.h>
#include <BH1750.h>

// Define relay pin
const int relayPin = D5;

// Define LDR pin
const int ldrPin = A0;

// Create an instance of the BH1750 class
BH1750 lightMeter;

// Task handles
TaskHandle_t readSensorsTaskHandle;
TaskHandle_t controlRelayTaskHandle;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize the BH1750 sensor
  Wire.begin(D1, D2);
  lightMeter.begin();
  
  // Set relay pin as output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  
  // Create tasks
  xTaskCreatePinnedToCore(readSensorsTask, "ReadSensorsTask", 2048, NULL, 1, &readSensorsTaskHandle, 0);
  xTaskCreatePinnedToCore(controlRelayTask, "ControlRelayTask", 2048, NULL, 1, &controlRelayTaskHandle, 1);
}

void loop() {
  // Main loop does nothing, tasks handle everything
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void readSensorsTask(void *pvParameters) {
  (void) pvParameters;
  
  while (true) {
    // Read ambient light from BH1750
    float lux = lightMeter.readLightLevel();
    Serial.print("Ambient Light (BH1750): ");
    Serial.print(lux);
    Serial.println(" lx");
    
    // Read LDR value
    int ldrValue = analogRead(ldrPin);
    Serial.print("LDR Value: ");
    Serial.println(ldrValue);
    
    // Delay between reads
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void controlRelayTask(void *pvParameters) {
  (void) pvParameters;
  
  while (true) {
    // Read sensor values
    float lux = lightMeter.readLightLevel();
    int ldrValue = analogRead(ldrPin);
    
    // Define thresholds
    float luxThreshold = 50.0; // Adjust as needed
    int ldrThreshold = 500; // Adjust as needed
    
    if (lux < luxThreshold || ldrValue < ldrThreshold) {
      digitalWrite(relayPin, HIGH); // Turn on relay
    } else {
      digitalWrite(relayPin, LOW); // Turn off relay
    }
    
    // Delay between control decisions
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
