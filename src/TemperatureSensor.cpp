/**
 * TemperatureSensor.cpp
 * Purpose: implements class TemperatureSensor
 *
 * @author Wall.Of.Death
 * @version 1.0 20160705
 */

#include "TemperatureSensor.h"
#include  "Third-Party-Code/DS18B20.cpp"


/**
 * TemperatureSensor::readSensor
 * @brief readSensor() returns the current temperature measured by the temperature-sensor
 * @return a Databuffer which contains temperature value in degree celsius, measured by the temperature-sensor
 */
DataBuffer TemperatureSensor::readSensor() {
    DataBuffer result;
    result.dataSource = "WeatherStation";
    result.useDataSource = true;

    result.data["Lufttemperatur_2m"] = readDS18B20(PIN_OF_DS18B20);

    return result;
}




/**
 * TemperatureSensor::readDS18B20
 * @brief read current measurement from DS18B20-sensor using 1-wire
 * @return returns current temperature in degree celsius
 */
double TemperatureSensor::readDS18B20(int pin_) {
    int temp;
    int loop;
    int Flag=0;
    unsigned long AcqDelay; // Acquisition time delay needed
    unsigned char Hresolution;

    DS18B20_Pins[0]=pin_;

    // Set up gpi pointer for direct register access

    //  set default mask
    PinMask = 0;

    for(loop=0;loop<4;loop++) {
        ModeMaskInput[loop]=0xffffffffL;
            ModeMaskOutput[loop]=0;
    }


    // set mask every pin
    temp = DS18B20_Pins[0];
    PinMask|= 1<<temp;
    ModeMaskInput[temp/10] &= ~(7<<((temp % 10) * 3));
    ModeMaskOutput[temp/10] |= (1<<((temp % 10) * 3));


    setup_io();

    ReadSensors();

    // set resolution
    Hresolution=DS18B20_Data[0].resolution;

    // now set timing according to the highesh resolution sensor.
    switch(Hresolution){
        case 9: AcqDelay= 94000;break;
        case 10: AcqDelay= 188000;break;
        case 11: AcqDelay= 375000;break;
            default: AcqDelay= 750000;
    }


    // request a temperature neasuremnt in the sensor

    set_max_priority();

    DoReset();

    // Start Acquisition
    WriteByte(DS18B20_SKIP_ROM);
    WriteByte(DS18B20_CONVERT_T);

    set_default_priority();

    //  wait  for the highest resolution probe
    usleep(AcqDelay);

    // get the measurement from sensor
    ReadSensors();

    if(DS18B20_Data[0].valid) {
        return DS18B20_Data[0].temperature;
    } else {
        printf("Bad CRC!\n");
        return 0;
    }

}
