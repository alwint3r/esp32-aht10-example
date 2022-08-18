#include <Arduino.h>
#include <Wire.h>
#include <AHT10.h>
#include <AHT10-Arduino-I2C-HAL.h>

static constexpr auto SENSOR_ADDR = 0x38;

AHT10::I2C::Arduino i2cHAL(&Wire, SENSOR_ADDR);
AHT10::Sensor sensor(&i2cHAL, delay);

void setup()
{
    Wire.begin();
    Serial.begin(9600);

    delay(3000);

    sensor.initialize(); // initialize();
    delay(100);
}

void loop()
{
    auto res = sensor.measure();
    if (res.status == AHT10::Measurement::MEASUREMENT_OK)
    {
        if (res.sensorStatus.isBusy())
        {
            Serial.printf("Sensor is busy!");
        }

        Serial.printf("Temperature: %f, Humidity: %f\r\n", res.temperature, res.humidity);
    }
    delay(1000);
}
