#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  randomSeed(analogRead(0));
}

void loop() {
  int16_t randomNumber[] = {random(30), random(30), random(30)};
  uint8_t sensor_ida = 1;
  uint8_t sensor_idb = 2;
  uint8_t marker = 0xFF;
  int32_t data = random(40);

  Serial.write(marker);
  Serial.write(sensor_ida);
  Serial.write((uint8_t*)&randomNumber[0], sizeof(randomNumber[0]));
  Serial.write((uint8_t*)&randomNumber[1], sizeof(randomNumber[1]));
  Serial.write((uint8_t*)&randomNumber[2], sizeof(randomNumber[2]));
  Serial.write(marker);
  Serial.write(sensor_idb);
  Serial.write((uint8_t*)&data, sizeof(data));

  delay(1000);
}
