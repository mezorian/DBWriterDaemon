/**
 * AirPressureSensor.h
 * Purpose: defines class AirPressureSensor
 *
 * @author Wall.Of.Death
 * @version 1.0 20160705
 */


#ifndef AIRPRESSURESENSOR_H
#define AIRPRESSURESENSOR_H

// --- TODO -- dummy code ---
#include <iostream>
#include <stdlib.h>

using namespace std;
// --- TODO -- dummy code ---

#include "SensorType.h"
#include "DataBuffer.h"

/**
 * AirPressureSensor class
 * @brief The AirPressureSensor class provides an implementation of readSensor() for the Sensor TODO
 *
 * The AirPressureSensor class is an implementation of the interface SensorType and implements the function readSensor().
 * The design of it is based on the design pattern 'strategy'.
 * In respect of this the AirPressureSensor-class is a so called 'ConcreteStrategy' within the Strategy-Pattern.
 */
class AirPressureSensor : public SensorType {
    public:
        /* --- constructor --- */
        AirPressureSensor() {};

        /* --- measuring --- */
        DataBuffer readSensor();
};

#endif // AIRPRESSURESENSOR_H
