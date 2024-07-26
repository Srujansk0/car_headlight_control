#include <ESP8266WiFi.h>
#include <DHT.h>

// Define the pins
#define LDR_PIN A0 // Analog pin for LDR
#define DHT_PIN D4 // Digital pin for DHT sensor

// Define the DHT sensor type
#define DHT_TYPE DHT11 // Use DHT22 if you have a DHT22 sensor

// Initialize DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

// FreeRTOS task handles
TaskHandle_t LDRTaskHandle;
TaskHandle_t DHTTaskHandle;

void setup() {
    Serial.begin(115200);
    dht.begin();

    // Create tasks
    xTaskCreatePinnedToCore(
        LDRTask,        // Task function
        "LDRTask",      // Task name
        2048,           // Stack size
        NULL,           // Parameter
        1,              // Priority
        &LDRTaskHandle, // Task handle
        0               // Core ID
    );

    xTaskCreatePinnedToCore(
        DHTTask,        // Task function
        "DHTTask",      // Task name
        2048,           // Stack size
        NULL,           // Parameter
        1,              // Priority
        &DHTTaskHandle, // Task handle
        1               // Core ID
    );
}

// Task to read LDR data
void LDRTask(void *pvParameters) {
    while (true) {
        int ldrValue = analogRead(LDR_PIN);
        Serial.print("LDR Value: ");
        Serial.println(ldrValue);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}

// Task to read DHT data
void DHTTask(void *pvParameters) {
    while (true) {
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature(); // Celsius

        if (isnan(humidity) || isnan(temperature)) {
            Serial.println("Failed to read from DHT sensor!");
        } else {
            Serial.print("Temperature: ");
            Serial.print(temperature);
            Serial.print(" °C, Humidity: ");
            Serial.print(humidity);
            Serial.println(" %");
        }

        vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2 seconds
    }
}

void loop() {
    // Empty. FreeRTOS tasks handle everything.
}
