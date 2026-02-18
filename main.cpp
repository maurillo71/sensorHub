#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "I2CReader.h"
#include "SensorHub.h"
#include "SensorDataDb.h"

#define MAX_SENSOR_REGISTER  0x0E

static const std::string  registerDescription[MAX_SENSOR_REGISTER] =
    {
        std::string("Nothing"),
        std::string("Temperature"),
        std::string("Light Red Low"),
        std::string("Light Red High"),
        std::string("Status"),
        std::string("On board Temperature"),
        std::string("On board humidity"),
        std::string("On board sensor error"),
        std::string("Bmp280 Temperature"),
        std::string("Bmp280 Pressure Low"),
        std::string("Bmp280 Pressure Mid"),
        std::string("Bmp280 Pressure High"),
        std::string("Bmp280 Status"),
        std::string("Human Detect"),
    };

std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

int main() {
    try {
        std::cout << "--- I2C Sensor Reading ---" << std::endl;

        // Replace "/dev/i2c-1" with your I2C device path and 0x50 with your device address
        // I2CDeviceReader reader("/dev/i2c-1", 0x17);
        SensorHub sensorHub("/dev/i2c-1", 0x17);

        std::cout << "\n--- Database Insertion Example ---" << std::endl;

        // 1. Initialize the database manager
        SensorDataDb db("sensor_hub_data.db");

        while (1)
        {
            uint8_t temp1 = sensorHub.getTemperature();
            uint8_t temp2 = sensorHub.getOnBoardTemperature();
            uint8_t temp3 = sensorHub.getBMP280Temperature();
            uint8_t status = sensorHub.getStatus();
            uint32_t pressure = sensorHub.getPressure();
            uint8_t humidity = sensorHub.getHumidity();
            uint16_t light = sensorHub.getLight();
            uint8_t humanDetect = sensorHub.getHumanDetect();

            std::cout << "Temperature: " << static_cast<int>(temp1) << std::endl;
            std::cout << "On Board Temperature: " << static_cast<int>(temp2) << std::endl;
            std::cout << "BMP280 Temperature: " << static_cast<int>(temp3) << std::endl;
            std::cout << "Status: " << static_cast<int>(status) << std::endl;
            std::cout << "Pressure: " << static_cast<int>(pressure) << std::endl;
            std::cout << "Humidity: " << static_cast<int>(humidity) << std::endl;
            std::cout << "Light: " << static_cast<int>(light) << std::endl;
            std::cout << "Human Detect: " << static_cast<int>(humanDetect) << std::endl;

            // for (auto idx = 0; idx < MAX_SENSOR_REGISTER; idx++)
            // {
            //     uint8_t data = reader.readByte(idx);

            //     std::cout << registerDescription[idx] << ": " << static_cast<int>(data) << std::endl;    
            // }


            // 2. Create a sample sensor reading
            SensorReading reading = {
                .datetime = getCurrentDateTime(),
                .temperature = temp1,
                .pressure = pressure,
                .humidity = humidity,
                .luminance = light
            };

            // 3. Insert the reading into the database
            db.insertReading(reading);

            sleep(10);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
