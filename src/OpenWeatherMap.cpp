/**
 * OpenWeatherMap.cpp
 * Purpose: implements class OpenWeatherMap
 *
 * @author Wall.Of.Death
 * @version 1.0 20160705
 */

#include "OpenWeatherMap.h"

/**
 * OpenWeatherMap::OpenWeatherMap
 * @brief constructor of OpenWeatherMap, initializes LogWriter
 */
OpenWeatherMap::OpenWeatherMap() {
    // create LogWriter-object
    log = LogWriter("OpenWeatherMap", PATH_OF_LOGFILE);
}

/**
 * OpenWeatherMap::readWebContent
 * @brief readWebContent() returns the current forecast-data read from http://openweathermap.org/
 * @return returns DataBuffer-object which contains the current forecast-data of the webcontent http://openweathermap.org/
 */
DataBuffer OpenWeatherMap::readWebContent() {

    HTTPRequest req;
    string jsonString = req.get(URL_OF_OPEN_WEATHER_MAP);
    DataBuffer openWeatherMapBuffer = jsonToDataBuffer(jsonString);
    return openWeatherMapBuffer;
}

/**
 * OpenWeatherMap::jsonToDataBuffer
 * @brief parses json (coming from OpenWeatherMap.com) to DataBuffer
 * @param jsonString_ json to convert to DataBuffer
 * @return returns DataBuffer which contains data from jsonString_
 */
DataBuffer OpenWeatherMap::jsonToDataBuffer(const string &jsonString_) {
    DataBuffer result;

    // get unix time (in seconds since 1970)
    // for current hour + 1 day
    // this is needed because we want the forecast
    // of now + 1 day

    // get current time in UTC
    // (open weather map timestamps are in UTC)
    QDateTime dateTime = QDateTime::currentDateTimeUtc();
    // delete minutes and seconds
    QTime time = dateTime.time();
    // round hours to 3-hours-steps
    int newHour = time.hour() - time.hour() % 3;
    time.setHMS(newHour,0,0);
    dateTime.setTime(time);
    // add one day
    dateTime = dateTime.addDays(1);
    // convert to unix time
    qint64 searchedUnixTime = dateTime.toMSecsSinceEpoch() / 1000.0;

    // set attributes of databuffer
    dateTime = dateTime.addSecs(-60*60*2);
    result.startDateTime.tm_year = dateTime.date().year();
    result.startDateTime.tm_mon  = dateTime.date().month();
    result.startDateTime.tm_mday = dateTime.date().day();
    result.startDateTime.tm_hour = dateTime.time().hour();
    result.startDateTime.tm_min  = dateTime.time().minute();
    result.startDateTime.tm_sec  = dateTime.time().second();
    result.endDateTime   = result.startDateTime;
    result.useDateTimes  = true;
    result.dataSource    = "Forecast";
    result.useDataSource = true;

    // load json
    QString jsonQString(jsonString_.c_str());
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonQString.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();

    // parse json from influxdb
    if (jsonObject.contains(QString("list"))) {
        QJsonArray jsonArray = jsonObject["list"].toArray();

        bool foundSearchedForecastTime = false;
        for (int i = 0; i < jsonArray.size(); i++) {
            int unixTimeOfJsonObject = jsonArray.at(i).toObject()["dt"].toInt();
            if (unixTimeOfJsonObject == searchedUnixTime) {
                foundSearchedForecastTime = true;
                if (jsonArray.at(i).toObject().contains("main")) {
                    QJsonObject jsonObject2 = jsonArray.at(i).toObject()["main"].toObject();

                    // temperature
                    if(jsonObject2.contains("temp")) {
                        result.data["Lufttemperatur_2m"] = jsonObject2["temp"].toDouble()-273.15;
                    }
                    // air pressure
                    if(jsonObject2.contains("pressure")) {
                        result.data["Luftdruck_2m"] = jsonObject2["pressure"].toDouble();
                    }
                    // humidity
                    if(jsonObject2.contains("humidity")) {
                        result.data["RelativeLuftfeuchte_2m"] = jsonObject2["humidity"].toDouble();
                    }
                } else {
                    log << SLevel(ERROR) << "Aborted parsing json to databuffer because json was not containing " <<
                           "temperature, pressure and humidity." << endl;
                    result.data.clear();
                    return result;
                }
                if (jsonArray.at(i).toObject().contains("wind")) {
                    QJsonObject jsonObject2 = jsonArray.at(i).toObject()["wind"].toObject();
                    // wind speed
                    if(jsonObject2.contains("speed")) {
                        result.data["Windgeschwindigkeit"] = jsonObject2["speed"].toDouble();
                    }
                    // wind direction
                    if(jsonObject2.contains("deg")) {
                        result.data["Windrichtung"] = jsonObject2["deg"].toDouble();
                    }
                } else {
                    log << SLevel(ERROR) << "Aborted parsing json to databuffer because json was not containing " <<
                           "wind-speed and wind-direction." << endl;
                    result.data.clear();
                    return result;
                }

                break;
            }

        }

        if (!foundSearchedForecastTime) {
            log << SLevel(ERROR) << "Aborted parsing json to databuffer, " <<
                   "because there was now forecast for datetime now + 1 day found." << endl;
        }


    } else {
        log << SLevel(ERROR) << "Aborted parsing json to databuffer because json was not containing " <<
               "list with forecast days." << endl;
        result.data.clear();
        return result;
    }
    return result;
}
