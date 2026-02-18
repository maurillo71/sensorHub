#include "SensorHub.h"

SensorHub::SensorHub(const std::string& devicePath, int deviceAddress) :
    I2CDeviceReader(devicePath, deviceAddress)
{
}

SensorHub::~SensorHub()
{
    
}
