/**
 * Sensor.h
 * Purpose: defines class Sensor
 *
 * @author Wall.Of.Death
 * @version 1.0 20160705
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "SensorType.h"
#include "DataBuffer.h"

/**
 * Sensor class
 * @brief The Sensor class describes a sensor of the type SensorType
 *
 * The design of the sensor-class is based on the design pattern 'strategy'.
 * In respect of this the sensor-class is the so called 'Context' within the Strategy-Pattern.
 *
 * The Sensor class provides access to different kinds of sensors, which are defined as implementations
 * of the interface SensorType.
 * By setSensorType() one can choose the type of the sensor one wants to access.
 * The function readSensor() returns the double value of the current sensor measurement
 */
class Sensor {
    public:
        /* --- constructor --- */
        Sensor() {};

        /* --- getter and setter --- */
        void setSensorType(SensorType *val_) { sensorType = val_; };
        SensorType* getSensorType()          { return sensorType; };

        /* --- measuring --- */
        DataBuffer readSensor();

    private:
        SensorType *sensorType;
};

#endif // SENSOR_H
