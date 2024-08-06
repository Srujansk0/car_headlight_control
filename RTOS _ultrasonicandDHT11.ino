#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>

// Define sensor pins
const int trigPin = D1; // Trigger pin for HC-SR04
const int echoPin = D2; // Echo pin for HC-SR04
const int dhtPin = D3;  // Data pin for DHT11

// Create DHT object
#define DHT_TYPE DHT11
DHT dht(dhtPin, DHT_TYPE);

// Define tasks stack size and priority
#define TASK_STACK_SIZE 2048
#define TASK_PRIORITY 1

// Function prototypes
void ultrasonicTask(void *pvParameters);
void dhtTask(void *pvParameters);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Set up pins for ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Create tasks
  xTaskCreate(ultrasonicTask, "Ultrasonic Task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
  xTaskCreate(dhtTask, "DHT Task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
}

void loop() {
  // Empty loop as tasks handle everything
  delay(1000);
}

void ultrasonicTask(void *pvParameters) {
  (void) pvParameters;
  while (true) {
    // Trigger the sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the echo time
    long duration = pulseIn(echoPin, HIGH);
    // Calculate distance in cm
    float distance = (duration / 2.0) * 0.0343;

    // Print distance to serial
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Delay before next read
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void dhtTask(void *pvParameters) {
  (void) pvParameters;
  while (true) {
    // Read temperature and humidity
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again)
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Print temperature and humidity to serial
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Delay before next read
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
