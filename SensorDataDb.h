#ifndef SENSOR_DATA_DB_H
#define SENSOR_DATA_DB_H

#include <string>
#include <stdexcept>
#include <sqlite3.h>
#include <cstdint>

/**
 * @struct SensorReading
 * @brief Holds a single sensor reading record.
 */
struct SensorReading {
    std::string datetime;
    uint8_t temperature;
    uint32_t pressure;
    uint8_t humidity;
    uint16_t luminance;
};

/**
 * @class SensorDataDb
 * @brief Manages a SQLite database for storing sensor data.
 */
class SensorDataDb {
public:
    explicit SensorDataDb(const std::string& dbPath);
    ~SensorDataDb();

    // Prohibit copy and assignment to prevent issues with the database handle
    SensorDataDb(const SensorDataDb&) = delete;
    SensorDataDb& operator=(const SensorDataDb&) = delete;

    void insertReading(const SensorReading& reading);

private:
    sqlite3* db;
    void createTable();
};

#endif // SENSOR_DATA_DB_H