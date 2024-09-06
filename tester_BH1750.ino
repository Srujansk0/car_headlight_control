#include <Wire.h>
#include <BH1750.h>  // BH1750 library for light sensor

// Initialize the BH1750 sensor
BH1750 lightMeter;

void setup() {
  // Start Serial communication
  Serial.begin(115200);

  // Initialize I2C communication
  Wire.begin();

  // Initialize BH1750 sensor
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Sensor initialized"));
  } else {
    Serial.println(F("Error initializing BH1750 sensor"));
  }
}

void loop() {
  // Read light intensity in lux
  float lux = lightMeter.readLightLevel();

  // Display the light intensity on Serial Monitor
  Serial.print(F("Light Intensity: "));
  Serial.print(lux);
  Serial.println(F(" lx"));

  // Delay for readability
  delay(1000);
}
