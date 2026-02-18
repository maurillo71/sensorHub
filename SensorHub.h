#ifndef SENSOR_HUB_H
#define SENSOR_HUB_H

#include <iostream>
#include <cstdint>
#include <string>
#include <sys/types.h>
#include "I2CReader.h"

class SensorHub : public I2CDeviceReader
{
public:
    SensorHub(const std::string& devicePath, int deviceAddress);
    ~SensorHub();

    /**
     * @brief Get the Temperature object
     * 
     * @return The read temperature
     */
    uint8_t getTemperature()
    {
        return readByte(TEMPERATURE_ADR);
    }
    
    /**
     * @brief Get the On Board Temperature object
     * 
     * @return The read onboard temperature
     */
    uint8_t getOnBoardTemperature()
    {
        return readByte(ON_BOARD_TEMPERATURE_ADR);
    }

    /**
     * @brief Get BMP280 temperature
     * 
     * @return the read temperature 
     */
    uint8_t getBMP280Temperature()
    {
        return readByte(BMP280_TEMPERATURE_ADR);
    }

    /**
     * @brief Get the BMP280 Temperature object
     * 
     * @return The read status 
     */
    uint8_t getBMP280Status()
    {
        return readByte(BMP280_STATUS_ADR);
    }

    /**
     * @brief Get the On Board Sensor Error object
     * 
     * @return The read onboard sensor error
     */
    uint8_t getOnBoardSensorError()
    {
        return readByte(ON_BOARD_SENSOR_ERROR_ADR);
    }

    uint8_t getHumanDetect()
    {
        return readByte(HUMAN_DETECT_ADR);
    }

    /**
     * @brief Get the Humidity object
     * 
     * @return The read humidity
     */
    uint8_t getHumidity()
    {
        return readByte(ON_BOARD_HUMIDITY_ADR);
    }
    
    /**
     * @brief Get the Light object
     * 
     * @return The read red light 
     */
    uint16_t getLight()
    {
        uint16_t readData;

        std::cout << "getLight: " << std::to_string(readByte(LIGHT_RED_HIGH_ADR)) << "--" << std::to_string(readByte(LIGHT_RED_LOW_ADR)) << std::endl;
        readData = static_cast<uint16_t>(readByte(LIGHT_RED_HIGH_ADR)) << 8;
        readData |= readByte(LIGHT_RED_LOW_ADR);

        return readData;
    }
    
    /**
     * @brief Get the Pressure object
     * 
     * @return the read pressure 
     */
    uint32_t getPressure()
    {
        uint32_t readData;

        std::cout << "getPressure: " << std::to_string(readByte(BMP280_PRESSURE_HIGH_ADR)) << "--" << 
                std::to_string(readByte(BMP280_PRESSURE_MID_ADR)) << "--" << std::to_string(readByte(BMP280_PRESSURE_LOW_ADR)) << std::endl;
        readData = static_cast<uint32_t>(readByte(BMP280_PRESSURE_HIGH_ADR)) << 16;
        std::cout << "1-readData: " << std::to_string(readData) << std::endl;
        readData |= static_cast<uint32_t>(readByte(BMP280_PRESSURE_MID_ADR)) << 8;
        std::cout << "2-readData: " << std::to_string(readData) << std::endl;
        readData |= readByte(BMP280_PRESSURE_LOW_ADR);
        std::cout << "3-readData: " << std::to_string(readData) << std::endl;
      
        return readData;
    }

    /**
     * @brief Get the Status object
     * 
     * @return uint8_t 
     */
    uint8_t getStatus()
    {
        return readByte(STATUS_ADR);
    }    
    
private:
    static const uint8_t TEMPERATURE_ADR=0x01;
    static const uint8_t LIGHT_RED_LOW_ADR=0x02;
    static const uint8_t LIGHT_RED_HIGH_ADR=0x03;
    static const uint8_t STATUS_ADR=0x04;
    static const uint8_t ON_BOARD_TEMPERATURE_ADR=0x05;
    static const uint8_t ON_BOARD_HUMIDITY_ADR=0x06;
    static const uint8_t ON_BOARD_SENSOR_ERROR_ADR=0x07;
    static const uint8_t BMP280_TEMPERATURE_ADR=0x08;
    static const uint8_t BMP280_PRESSURE_LOW_ADR=0x09;
    static const uint8_t BMP280_PRESSURE_MID_ADR=0x0A;
    static const uint8_t BMP280_PRESSURE_HIGH_ADR=0x0B;
    static const uint8_t BMP280_STATUS_ADR=0x0C;
    static const uint8_t HUMAN_DETECT_ADR=0x0D;
};

#endif // SENSOR_HUB_H