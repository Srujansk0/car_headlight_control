#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include <FreeRTOS.h>

// Define pins and thresholds
const int lightSensorPin = A0;  // Pin connected to the light sensor (TEMT6000)
const int headlightPin = 9;     // Pin connected to headlight control (PWM pin)

int lightThreshold = 200;       // Threshold for detecting oncoming headlights
int ambientThreshold = 50;      // Threshold for ambient light to consider it as night

BH1750 ambientLightSensor;

// Global variables for sensor data
volatile int lightIntensity = 0;
volatile float ambientLightLevel = 0.0;

void readLightSensorTask(void *pvParameters) {
  while (1) {
    lightIntensity = analogRead(lightSensorPin);
    vTaskDelay(pdMS_TO_TICKS(100));  // Read every 100 ms
  }
}

void readAmbientLightSensorTask(void *pvParameters) {
  while (1) {
    ambientLightLevel = ambientLightSensor.readLightLevel();
    vTaskDelay(pdMS_TO_TICKS(200));  // Read every 200 ms
  }
}

void controlHeadlightTask(void *pvParameters) {
  while (1) {
    // Check if it's dark enough for headlights
    if (ambientLightLevel < ambientThreshold) {
      // If an oncoming vehicle is detected, dim the headlights
      if (lightIntensity > lightThreshold) {
        analogWrite(headlightPin, 128); // Dim the headlights to 50% brightness
      } else {
        analogWrite(headlightPin, 255); // Full brightness
      }
    } else {
      // Daytime, no need to dim headlights
      analogWrite(headlightPin, 0); // Turn off headlights
    }
    vTaskDelay(pdMS_TO_TICKS(100));  // Check every 100 ms
  }
}

void setup() {
  pinMode(lightSensorPin, INPUT);
  pinMode(headlightPin, OUTPUT);
  
  Wire.begin();
  ambientLightSensor.begin();

  // Create FreeRTOS tasks
  xTaskCreate(readLightSensorTask, "Read Light Sensor", 1000, NULL, 1, NULL);
  xTaskCreate(readAmbientLightSensorTask, "Read Ambient Light Sensor", 1000, NULL, 1, NULL);
  xTaskCreate(controlHeadlightTask, "Control Headlight", 1000, NULL, 1, NULL);

  // Start the scheduler
  vTaskStartScheduler();
}

void loop() {
  // The loop will be empty since RTOS tasks are handling everything
}
