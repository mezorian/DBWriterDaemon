/**
 * AirPressureSensor.cpp
 * Purpose: implements class AirPressureSensor
 *
 * @author Wall.Of.Death
 * @version 1.0 20160705
 */

#include "AirPressureSensor.h"

/**
 * AirPressureSensor::readSensor
 * @brief readSensor() returns the current air pressure measured by the irpressure-sensor
 * @return a DataBuffer which contains a air pressure-value hectopascal, measured by the airpressure-sensor
 */
DataBuffer AirPressureSensor::readSensor() {
    DataBuffer result;
    result.dataSource = "WeatherStation";
    result.useDataSource = true;

    // --- TODO -- dummy code ---
    cout << "read AirPressure sensor" << endl;
    result.data["Luftdruck_2m"] = (rand() % 400) + 800 ;
    // --- TODO -- dummy code ---

    return result;
}
