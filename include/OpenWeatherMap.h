/**
 * OpenWeatherMap.h
 * Purpose: defines class OpenWeatherMap
 *
 * @author Wall.Of.Death
 * @version 1.0 20160705
 */


#ifndef OPENWEATHERMAP_H
#define OPENWEATHERMAP_H

// --- TODO -- dummy code ---
#include <iostream>
#include <stdlib.h>

using namespace std;
// --- TODO -- dummy code ---

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
// --
#include "WebContentType.h"
#include "HTTPRequest.h"
#include "Config.h"

/**
 * OpenWeatherMap class
 * @brief The OpenWeatherMap class provides an implementation of readWebContent() for the WebContent http://openweathermap.org/
 *
 * The OpenWeatherMap class is an implementation of the interface WebContentType and implements the function readWebContent().
 * The design of it is based on the design pattern 'strategy'.
 * In respect of this the OpenWeatherMap-class is a so called 'ConcreteStrategy' within the Strategy-Pattern.
 */
class OpenWeatherMap : public WebContentType {
    public:
        /* --- constructor --- */
        OpenWeatherMap();

        /* --- measuring --- */
        DataBuffer readWebContent ();
    private:
        DataBuffer jsonToDataBuffer(const string& jsonString_);
        LogWriter log;
};

#endif // OPENWEATHERMAP_H
