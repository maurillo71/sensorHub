#include "SensorDataDb.h"
#include <iostream>

SensorDataDb::SensorDataDb(const std::string& dbPath) : db(nullptr) {
    if (sqlite3_open_v2(dbPath.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr)) {
        std::string errMsg = "Can't open database: " + std::string(sqlite3_errmsg(db));
        sqlite3_close(db); // Can be called on a failed open handle
        throw std::runtime_error(errMsg);
    }
    createTable();
}

SensorDataDb::~SensorDataDb() {
    if (db) {
        sqlite3_close(db);
    }
}

void SensorDataDb::createTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS sensor_readings ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "datetime TEXT NOT NULL,"
                      "temperature INTEGER NOT NULL,"
                      "pressure INTEGER NOT NULL,"
                      "humidity INTEGER NOT NULL,"
                      "luminance INTEGER NOT NULL);";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, 0, &errMsg) != SQLITE_OK) {
        std::string error = "SQL error creating table: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
}

void SensorDataDb::insertReading(const SensorReading& reading) {
    const char* sql = "INSERT INTO sensor_readings (datetime, temperature, pressure, humidity, luminance) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
    }

    // Bind values to the prepared statement
    // Using sqlite3_bind_* functions is the recommended way to prevent SQL injection.
    sqlite3_bind_text(stmt, 1, reading.datetime.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, reading.temperature);
    sqlite3_bind_int(stmt, 3, reading.pressure);
    sqlite3_bind_int(stmt, 4, reading.humidity);
    sqlite3_bind_int(stmt, 5, reading.luminance);

    // Execute the statement
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::string errMsg = "Failed to execute statement: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt); // Clean up statement
        throw std::runtime_error(errMsg);
    }

    // Finalize the statement to release resources
    sqlite3_finalize(stmt);

    std::cout << "Successfully inserted a new sensor reading." << std::endl;
}