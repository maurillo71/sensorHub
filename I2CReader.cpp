#include "iostream"
#include "I2CReader.h"

I2CDeviceReader::I2CDeviceReader(const std::string& devicePath, int deviceAddress)
    : fileDescriptor(-1), deviceAddress(deviceAddress)
{
    openDevice(devicePath);
    setDeviceAddress();
}

I2CDeviceReader::~I2CDeviceReader()
{
    std::cout << "Close I2C Device!" << std::endl;
    closeDevice();
}

void I2CDeviceReader::openDevice(const std::string& devicePath) {
    fileDescriptor = open(devicePath.c_str(), O_RDWR);
    if (fileDescriptor < 0) {
        throw std::runtime_error("Failed to open I2C device: " + devicePath);
    }
}

void I2CDeviceReader::setDeviceAddress() {
    if (ioctl(fileDescriptor, I2C_SLAVE, deviceAddress) < 0) {
        throw std::runtime_error("Failed to set I2C device address");
    }
}

void I2CDeviceReader::closeDevice() {
    if (fileDescriptor >= 0) {
        close(fileDescriptor);
    }
}

uint8_t I2CDeviceReader::readByte(uint8_t registerAddress) {
    // Write the register address to the device
    if (write(fileDescriptor, &registerAddress, 1) != 1) {
        throw std::runtime_error("Failed to write register address to I2C device");
    }

    // Read a single byte from the device
    uint8_t data;
    if (read(fileDescriptor, &data, 1) != 1) {
        throw std::runtime_error("Failed to read data from I2C device");
    }

    return data;
}