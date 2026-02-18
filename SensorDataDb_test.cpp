#include "SensorDataDb.h"
#include <gtest/gtest.h>
#include <sqlite3.h>
#include <string>
#include <memory>

// Test fixture for SensorDataDb tests
class SensorDataDbTest : public ::testing::Test {
protected:
    void SetUp() override {
        dbPath = "test_sensor_data.db";
        // Ensure no previous test database exists
        remove(dbPath.c_str());
    }

    void TearDown() override {
        remove(dbPath.c_str());
    }

    std::string dbPath;
};

// Test case for inserting a sensor reading
TEST_F(SensorDataDbTest, InsertReading) {
    // 1. Setup
    SensorDataDb sensorDb(dbPath);
    SensorReading reading = {"2023-10-27 10:00:00", 25, 101325, 45, 5000};

    // 2. Exercise
    sensorDb.insertReading(reading);

    // 3. Verify
    sqlite3* db;
    ASSERT_EQ(sqlite3_open(dbPath.c_str(), &db), SQLITE_OK);

    const char* sql = "SELECT datetime, temperature, pressure, humidity, luminance FROM sensor_readings ORDER BY id DESC LIMIT 1;";
    sqlite3_stmt* stmt;
    ASSERT_EQ(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr), SQLITE_OK);

    // Check if a row is returned
    ASSERT_EQ(sqlite3_step(stmt), SQLITE_ROW);

    // Verify the data in the row
    EXPECT_EQ(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))), reading.datetime);
    EXPECT_EQ(sqlite3_column_int(stmt, 1), reading.temperature);
    EXPECT_EQ(sqlite3_column_int(stmt, 2), reading.pressure);
    EXPECT_EQ(sqlite3_column_int(stmt, 3), reading.humidity);
    EXPECT_EQ(sqlite3_column_int(stmt, 4), reading.luminance);

    // 4. Teardown
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Test case for constructor failing with an invalid path
TEST_F(SensorDataDbTest, ConstructorFailsWithInvalidPath) {
    // An invalid path (e.g., a directory that doesn't exist, or a path with
    // insufficient permissions) should cause the constructor to throw.
    // Here, we use a path that is likely not writable.
    std::string invalidPath = "/non_existent_dir/test.db";
    EXPECT_THROW(SensorDataDb sensorDb(invalidPath), std::runtime_error);
}

// Test case for inserting multiple sensor readings
TEST_F(SensorDataDbTest, InsertMultipleReadings) {
    // 1. Setup
    SensorDataDb sensorDb(dbPath);
    SensorReading reading1 = {"2023-10-27 10:00:00", 25, 101325, 45, 5000};
    SensorReading reading2 = {"2023-10-27 10:01:00", 26, 101330, 46, 5010};

    // 2. Exercise
    sensorDb.insertReading(reading1);
    sensorDb.insertReading(reading2);

    // 3. Verify
    sqlite3* db;
    ASSERT_EQ(sqlite3_open(dbPath.c_str(), &db), SQLITE_OK);

    const char* sql = "SELECT COUNT(*) FROM sensor_readings;";
    sqlite3_stmt* stmt;
    ASSERT_EQ(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr), SQLITE_OK);

    // Check if a row is returned
    ASSERT_EQ(sqlite3_step(stmt), SQLITE_ROW);

    // Verify the count
    EXPECT_EQ(sqlite3_column_int(stmt, 0), 2);

    // 4. Teardown
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
