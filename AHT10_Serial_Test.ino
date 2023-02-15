#include <Wire.h>

#define AHT10_ADDRESS 0x38

void setup() {
  Wire.begin();

  Serial.begin(9600);
  Serial.println("AHT10 Test");
}

void loop() {
  Wire.beginTransmission(AHT10_ADDRESS);
  Wire.write(0xAC);
  Wire.write(0x33);
  Wire.write(0x00);
  Wire.endTransmission();

  delay(100);

  Wire.requestFrom(AHT10_ADDRESS, 6);
  uint8_t data[6];
  for (int i = 0; i < 6; i++) {
    data[i] = Wire.read();
  }

  uint32_t hum = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
  uint32_t temp = ((uint32_t)data[3] & 0x0F) << 16 | (data[4] << 8) | data[5];

  float humidity = (float)hum * 100 / 0x100000;
  float temperature = (float)temp * 200 / 0x100000 - 50;

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  delay(5000);
}
