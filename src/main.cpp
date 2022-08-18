#include <Arduino.h>
#include <Wire.h>

void measure();
void initialize();

static constexpr auto SENSOR_ADDR = 0x38;

void setup()
{
    Wire.begin();
    Serial.begin(9600);

    delay(3000);

    initialize();
    delay(100);
}

void loop()
{
    measure();
    delay(1000);
}

void initialize()
{
    auto cmd = 0b11100001;

    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(cmd);
    auto res = Wire.endTransmission();

    Serial.printf("Initialization ack: %d\r\n", res == 0);
}

void measure()
{
    auto cmd = 0b10101100;

    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(cmd);
    Wire.write(0b00110011);
    Wire.write(0);
    auto res = Wire.endTransmission();

    Serial.printf("Measurement trigger ack: %d\r\n", res == 0);

    delay(80);

    auto expected = 6;
    auto received = Wire.requestFrom(SENSOR_ADDR, expected);
    if (received < 6)
    {
        Serial.printf("Requested: %d, received: %d\r\n", 3, expected, received);
        return;
    }

    uint8_t status = Wire.read();
    Serial.print("Status byte: ");
    Serial.println(status, BIN);

    uint8_t humidityMsb = Wire.read();
    uint8_t humidityLsb = Wire.read();
    uint8_t humidityTemperatureHalf = Wire.read();
    
    uint8_t temperatureMsb = Wire.read();
    uint8_t temperatureLsb = Wire.read();

    uint32_t sHumidity = (humidityMsb << 16) | (humidityLsb << 8) | humidityTemperatureHalf;
    sHumidity >>= 4;

    uint32_t sTemperature = ((humidityTemperatureHalf & 0b00001111) << 16) | temperatureMsb << 8 | temperatureLsb;


    Serial.printf("STemperature: %ld, SHumidity: %ld\r\n", sTemperature, sHumidity);

    double humidity = (sHumidity / 1048576.0) * 100;
    double temperature = (sTemperature / 1048576.0) * 200 - 50;

    Serial.printf("Temperature: %f, Humditiy: %f\r\n", temperature, humidity);
}