#ifndef I2C_DEVICE_READER_H
#define I2C_DEVICE_READER_H

#include <string>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

class I2CDeviceReader {
public:
    I2CDeviceReader(const std::string& devicePath, int deviceAddress);
    ~I2CDeviceReader();

    // Reads a single byte from the I2C device
    uint8_t readByte(uint8_t registerAddress);

private:
    int fileDescriptor;
    int deviceAddress;

    void openDevice(const std::string& devicePath);
    void setDeviceAddress();
    void closeDevice();
};

#endif // I2C_DEVICE_READER_H