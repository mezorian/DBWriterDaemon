/**
 * DBInterface.cpp
 * Purpose: implements class DBInterface
 *
 * @author Wall.Of.Death
 * @version 1.0 20160704
 */


#include "DBInterface.h"

/**
 * DBInterface::init
 * @brief creates a LogWriter-object
 */
void DBInterface::init() {
    createIfNotCreatedDataBase();

    // create LogWriter-object
    log = LogWriter("DBInterface", PATH_OF_LOGFILE);

    log << SLevel(INFO) << "Initialized DBInterface with url : " << URL_OF_DATABASE << "." << endl;

}

/**
 * DBInterface::writeToDataBase
 * @brief writeToDataBase writes content of dataBuffer_ to database
 * @param dataBuffer_ data which is written to database
 */
void DBInterface::writeToDataBase(DataBuffer& dataBuffer_) {
    if (readStatusOK()) {

        stringstream httpRequestUrl;

        httpRequestUrl << URL_OF_DATABASE << "/write?db=" << NAME_OF_DATBASE << "&precision=s";
        stringstream httpRequestPostFields;
        if (!dataBuffer_.useDataSource) {
            log << SLevel(ERROR) << "Aborted writing to database because there was either no DataSource specified" <<
             " or the useDataSource-flag was not set to true." << endl;
        } else {
            string dataSource = cleanString(dataBuffer_.dataSource);
            httpRequestPostFields << "point,DataSource=" << dataSource << " ";

            bool printComma = false;
            typedef std::map<string, double>::iterator it_type;
            for(it_type iterator = dataBuffer_.data.begin(); iterator != dataBuffer_.data.end(); iterator++) {
                if (printComma) {
                    httpRequestPostFields << ",";
                } else {
                    printComma = true;
                }
                string name = cleanString(iterator->first);
                double value = cutValueToInfluxDBRange(iterator->second);
                httpRequestPostFields << name << "=" << value;
            }
            // create datetime-string
            if (dataBuffer_.useDateTimes) {
                if (dataBuffer_.startDateTime.tm_year <= 1971) {
                    log << SLevel(ERROR) << "Aborted writing to database because of invalid datetime. " <<
                    "Please use only years bigger than 1971." << endl;
                    setDBFailure(true);
                    return;
                } else {
                    string startDateTime = cTimeToString(dataBuffer_.startDateTime,true);
                    httpRequestPostFields << " " << startDateTime;
                }
            } else {
                // if no date-time is specified use local time (cut down to current hour)
                int currentLocalDateTime = getCurrentDateTimeAsUnixTime();
                string startDateTime = to_string(currentLocalDateTime);
                httpRequestPostFields << " " << startDateTime;
            }
            HTTPRequest req;
            bool noFailure = req.post(httpRequestUrl.str(),httpRequestPostFields.str());
            setDBFailure(!noFailure);
        }
    } else {
        log << SLevel(ERROR) << "Aborted writing to database because of status not OK" << endl;
    }
}

/**
 * DBInterface::readFromDataBase
 * @brief readFromDataBase reads the data which is requested by dataBuffer_ from database
 * @param dataBuffer_ requested data (is only filled with requested strings)
 * @return returns the requested dataBuffer_ which now contains requested data
 */
vector<DataBuffer> DBInterface::readFromDataBase(DataBuffer& dataBuffer_) {
    // create empty result
    vector<DataBuffer> result;

    if (readStatusOK()) {

        // create url-string for select ... from ...
        stringstream httpRequestUrl;
        httpRequestUrl << URL_OF_DATABASE << "/query?pretty=true&db=" << NAME_OF_DATBASE << "&q=SELECT+"; // << "&precision=s";

        // iterate all requested fields
        bool printComma = false;
        typedef std::map<string, double>::iterator it_type;
        for(it_type iterator = dataBuffer_.data.begin(); iterator != dataBuffer_.data.end(); iterator++) {
            if (printComma) {
                httpRequestUrl << ",";
            } else {
                printComma = true;
            }
            string name = cleanString(iterator->first);
            httpRequestUrl << name << "+";
        }

        // add requested datasource
        if (!dataBuffer_.useDataSource) {
            log << SLevel(ERROR) << "Aborted reading from database because there was either no DataSource specified" <<
            " or the useDataSource-flag was not set to true." << endl;
        } else {
            string dataSource = cleanString(dataBuffer_.dataSource);
            httpRequestUrl << "FROM+point+where+DataSource+=+'" << dataSource << "'";

            // add requested datetime-range
            if (dataBuffer_.useDateTimes) {
                if ( (dataBuffer_.startDateTime.tm_year <= 1971) ||
                     (dataBuffer_.endDateTime.tm_year   <= 1971) ){
                    log << SLevel(ERROR) << "Aborted reading from database because of invalid datetime. " <<
                    "Please use only years bigger than 1971." << endl;
                    setDBFailure(true);
                    return result;
                }
                string startDateTime = cTimeToString(dataBuffer_.startDateTime,false);
                string   endDateTime = cTimeToString(  dataBuffer_.endDateTime,false);
                httpRequestUrl << "+and+time+>=+'" << startDateTime << "'";
                httpRequestUrl << "+and+time+<=+'" <<   endDateTime << "'";
            } else {
                // if no date-time is specified use local time (cut down to current hour)
                struct tm currentLocalDateTime = getCurrentDateTime();
                string startDateTime = cTimeToString(currentLocalDateTime,false);
                string   endDateTime = startDateTime;
                httpRequestUrl << "+and+time+>=+'" << startDateTime << "'";
                httpRequestUrl << "+and+time+<=+'" <<   endDateTime << "'";
            }


            // execute request
            HTTPRequest req;
            string answerJSON = req.get(httpRequestUrl.str());
            setDBFailure(answerJSON == "");

            // convert json to vector of DataBuffer
            result = jsonToDataBufferVector(answerJSON,dataBuffer_.dataSource);

        }
    } else {
        log << SLevel(ERROR) << "Aborted reading from database because of status not OK" << endl;
    }

    // return
    return result;
}

/**
 * DBInterface::writeStatusOK
 * @brief writes the boolean value statusOK_ to database
 * @param statusOK_ boolean value which is wirtten to the database
 */
void DBInterface::writeStatusOK(bool statusOK_) {
    stringstream httpRequestUrl;
    stringstream httpRequestPostFields;
    HTTPRequest req;

    // try to delete old value from influxDB
    // (post writes an error to log, if there is nothing to delete)
    httpRequestUrl << URL_OF_DATABASE << "/query?db=" << NAME_OF_DATBASE;
    httpRequestPostFields << "q=drop measurement statusOK";
    req.post(httpRequestUrl.str(),httpRequestPostFields.str());

    // write new value to influxDB
    stringstream httpRequestUrl2;
    stringstream httpRequestPostFields2;
    httpRequestUrl2 << URL_OF_DATABASE << "/write?db=" << NAME_OF_DATBASE << "&precision=s";

    httpRequestPostFields2 << "statusOK value=" << statusOK_;

    bool noFailure = req.post(httpRequestUrl2.str(),httpRequestPostFields2.str());
    setDBFailure(!noFailure);
    if (!noFailure) {
        log << SLevel(ERROR) << "Aborted writing statusOK to database because http-post was not possible." << endl;
    }
}

/**
 * DBInterface::readStatusOK
 * @brief reads the boolean value statusOK from database and returns it.
 * @return returns the value of the boolean value statusOK
 */
bool DBInterface::readStatusOK() {
    // create url-string for select ... from ...
    stringstream httpRequestUrl;
    HTTPRequest req;

    httpRequestUrl << URL_OF_DATABASE << "/query?pretty=true&db=" << NAME_OF_DATBASE << "&q=SELECT+";
    httpRequestUrl << "value+FROM+statusOK";

    string answerJSON = req.get(httpRequestUrl.str());
    setDBFailure(answerJSON == "");

    vector<DataBuffer> dataBufferVec = jsonToDataBufferVector(answerJSON,"");

    double result = dataBufferVec[0].data["value"];

    return (result != 0);
}

/**
 * DBInterface::getDBFailure
 * @brief returns value of private member variable dbFailure
 * @return value of private member variable dbFailure
 */
bool DBInterface::getDBFailure() {
    return dbFailure;
}

/**
 * DBInterface::createIfNotCreatedDataBase
 * @brief creates database defined by NAME_OF_DATBASE in config.h if not already created
 */
void DBInterface::createIfNotCreatedDataBase() {
    HTTPRequest request;
    request.post(URL_OF_DATABASE + "/query?q=create+database+"+NAME_OF_DATBASE+"&db=_internal","");
}

/**
 * DBInterface::cleanString
 * @brief removes every char that is not an alphabetic character, a number or '_'
 * @param stringToClean_ the string to clean
 * @return returns stringToClean_ without anything but character, numbers and '_'
 */
string DBInterface::cleanString(const string &stringToClean_) {
    string result = stringToClean_;
    // remove everythin that not is not alphanum and that is not _
    result.erase(
                  remove_if( result.begin(), result.end(),
                             [](char c) { return !(isalnum(c) || (c == '_') ) ; }
                           ),
                  result.end()
                );

    return result;
}

/**
 * DBInterface::cutValueToInfluxDBRange
 * @brief cuts val_ to the range INFLUXDB_MAX <= val_ >= INFLUXDB_MIN
 * @param val_ value to cut
 * @return returns INFLUXDB_MAX / INFLUXDB_MIN if value is bigger/smaller as INFLUXDB_MAX/INFLUXDB_MIN
 */
double DBInterface::cutValueToInfluxDBRange(double val_) {
    if (val_ > INFLUXDB_MAX) {
        return INFLUXDB_MAX;
    } else if (val_ < INFLUXDB_MIN) {
        return INFLUXDB_MIN;
    } else {
        return val_;
    }
}

/**
 * DBInterface::deletePaddingZeros
 * @brief removes leading padding zeros from a given string
 * @param stringWithPaddingZeros_ string with padding zeros
 * @return return stringWithPaddingZeros_ without padding zeros
 *
 * NOTICE : if stringWithPaddingZeros_ consists of only zeros
 *          the function returns "0"
 */
string DBInterface::deletePaddingZeros(const string &stringWithPaddingZeros_) {
    string result = stringWithPaddingZeros_;
    // erase leading zeros
    result.erase(0, result.find_first_not_of('0'));
    // if every char was a zero
    // return only one zero
    if (result == "") {
        result = "0";
    }
    return result;
}

/**
 * DBInterface::cTimeToString
 * @brief convert a ctime struct to either a formatted string or the unix-time as string
 * @param datetime_ the struct which is to convert
 * @param inUnixTime_ function returns unix-time as string if true, otherwise returns a formatted DateTimeString
 * @return returns either a formatted string or the unix-time as string, depending on inUnixTime_
 *
 * This function can convert a ctime struct to either a formatted data-time-string
 *   e.g. 2016-08-09T16:40:57Z
 * or to the unix-time (seconds since 1970) as string
 *   e.g. 1434055562
 *
 * NOTICE : only the tm-fields tm_year, tm_mon, tm_mday, tm_hour, tm_min, tm_sec are supported
 * NOTICE : always set tm-fields to EXACTLY the values you want to see in string!
 *          all conversion because of zero-indexes or 1900-offset are calculated inside the function
 *
 * for example :
 *
 *  tm_sec  = 2;    // seconds
 *  tm_min  = 46;   // minutes
 *  tm_hour = 20;   // hours
 *  tm_mday = 11;   // day
 *  tm_mon  = 6;    // month
 *  tm_year = 2015; // Year
 *
 *  this returns the output
 *   2015-06-11T20:46:02Z or 1434055562
 */
string DBInterface::cTimeToString(tm datetime_, bool inUnixTime_) {
    stringstream dateTimeString;

    if (inUnixTime_) {
        // convert to number of seconds since 1979 (unix-time)
        // e.g. 1434055562
        if (datetime_.tm_hour == 0) {

        }

        datetime_.tm_hour += 2;    // hours (0 to 23)
        datetime_.tm_mon  -= 1;    // month (0 bis 11)
        datetime_.tm_year -= 1900; // Year (calender-year minus 1900)
        datetime_.tm_isdst = 1;    // converting us-summer-time

        time_t secondsSince1970 = mktime(&datetime_);
        dateTimeString << secondsSince1970;
    } else {
        // convert to formatted string
        // e.g. 2016-08-09T16:40:57Z
        dateTimeString << datetime_.tm_year << "-" ;
        dateTimeString << setfill('0') << setw(2);
        dateTimeString << datetime_.tm_mon  << "-" ;
        dateTimeString << setfill('0') << setw(2);
        dateTimeString << datetime_.tm_mday << "T" ;
        dateTimeString << setfill('0') << setw(2);
        dateTimeString << datetime_.tm_hour << ":" ;
        dateTimeString << setfill('0') << setw(2);
        dateTimeString << datetime_.tm_min  << ":" ;
        dateTimeString << setfill('0') << setw(2);
        dateTimeString << datetime_.tm_sec  << "Z" ;
    }
    return dateTimeString.str();
}

/**
 * DBInterface::stringToCTime
 * @brief parse formatted date-time-string to ctime
 * @param dateTimeString_ formatted date-time-string
 * @return returns a struct tm (ctime) which contains the datetime of dateTimeString_
 *
 * converts a date-time-string which MUST be formatted like :
 * yyyy-mm-ddThh:mm:ssZ
 * see the following example on how the string has to be formatted :
 * 2016-08-09T16:40:57Z
 *
 * NOTICE : only the tm-fields tm_year, tm_mon, tm_mday, tm_hour, tm_min, tm_sec are supported
 *          the other fields are ALWAYS set to :
 *
 *              tm_wday   = 0;
 *              tm_yday   = 0;
 *              tm_zone   = "";
 *              tm_gmtoff = 0;
 *              tm_isdst  = 0;
 */
tm DBInterface::stringToCTime(const string &dateTimeString_) {
    //2016-08-09T16:40:57Z
    struct tm result;
    stringstream temp;
    string substring;
    // parse year
    substring = dateTimeString_.substr(0,4);
    result.tm_year = stoi(substring);
    // parse month
    substring = dateTimeString_.substr(5,2);
    substring = deletePaddingZeros(substring);
    result.tm_mon = stoi(substring);
    // parse day
    substring = dateTimeString_.substr(8,2);
    substring = deletePaddingZeros(substring);
    result.tm_mday = stoi(substring);
    // parse hour
    substring = dateTimeString_.substr(11,2);
    substring = deletePaddingZeros(substring);
    result.tm_hour = stoi(substring);
    // parse minutes
    substring = dateTimeString_.substr(14,2);
    substring = deletePaddingZeros(substring);
    result.tm_min = stoi(substring);
    // parse seconds
    substring = dateTimeString_.substr(17,2);
    substring = deletePaddingZeros(substring);
    result.tm_sec = stoi(substring);

    // set unused tm_fields
    result.tm_wday   = 0;
    result.tm_yday   = 0;
    result.tm_zone   = "";
    result.tm_gmtoff = 0;
    result.tm_isdst  = 0;

    return result;
}

/**
 * DBInterface::getCurrentDateTime
 * @brief gets current local time and outputs it
 * @return returns current local time as tm struct
 */
tm DBInterface::getCurrentDateTime(bool cutToHours_) {
    // create time variable
    time_t  secondsSince1970;
    struct tm *result;
    time ( &secondsSince1970 );
    result = localtime ( &secondsSince1970 );
    result->tm_year += 1900;
    result->tm_mon  += 1;
    result->tm_isdst = 0;
    if (cutToHours_) {
        result->tm_min = 0;
        result->tm_sec = 0;
    }
    return *result;
}


/**
 * DBInterface::getCurrentDateTime
 * @brief gets current local time and outputs it
 * @return returns current local time as seconds since 1970
 */
int DBInterface::getCurrentDateTimeAsUnixTime(bool cutToHours_) {
    struct tm temp = getCurrentDateTime(cutToHours_);
    temp.tm_mon  -= 1;    // month (0 bis 11)
    temp.tm_year -= 1900; // Year (calender-year minus 1900)
    temp.tm_isdst = 1;    // converting us-summer-time

    time_t secondsSince1970 = mktime(&temp);
    return secondsSince1970;
}

/**
 * DBInterface::jsonToDataBufferVector
 * @brief iterate through JSON and return data inside json_ as vector of DataBuffer
 * @param json_ JSON string to convert
 * @return returns a vector of DataBuffer which contains the data inside json_
 *
 * This function can convert a json received from either InfluxDB or TODO
 * to a vector of DataBuffers.
 * If the JSONs origin was InfluxDB, the return values contains a DataBuffer for
 * every DateTime which was requested from InfluxDB.
 */
vector<DataBuffer> DBInterface::jsonToDataBufferVector(const string &json_, const string& dataSource_) {
    vector<DataBuffer> result;
    QString jsonQString(json_.c_str());
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonQString.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();

    // parse json from influxdb
    if (jsonObject.contains(QString("results"))) {
        QJsonArray tempArray = jsonObject["results"].toArray();
        QJsonObject tempObject = tempArray.first().toObject();
        if (tempObject.contains(QString("series"))) {
            tempArray = tempObject["series"].toArray();
            tempObject = tempArray.first().toObject();
            if (tempObject.contains(QString("columns")) &&
                tempObject.contains(QString("values" )) ){
                QJsonArray names  = tempObject["columns"].toArray();
                QJsonArray values = tempObject["values" ].toArray();

                // iterate through all datasets
                typedef QJsonArray::iterator it_type;
                for(it_type iterator = values.begin(); iterator != values.end(); iterator++) {
                    QJsonArray dataSet = values.at(iterator.i).toArray();
                    DataBuffer tempDataBuffer;
                    // iterate to all names/values in a dataset
                    for(it_type iterator2 = dataSet.begin(); iterator2 != dataSet.end(); iterator2++) {

                        // get name
                        string name = names.at(iterator2.i).toString().toStdString();
                        // get value
                        QJsonValue valueJSON = dataSet.at(iterator2.i);

                        // set time
                        if (name == "time") {
                            struct tm time = stringToCTime(valueJSON.toString().toStdString());
                            tempDataBuffer.useDateTimes = true;
                            tempDataBuffer.startDateTime = time;
                            tempDataBuffer.endDateTime   = time;
                        } else {
                            // set values
                            double valueDouble = valueJSON.toDouble();
                            tempDataBuffer.data[name] = cutValueToInfluxDBRange(valueDouble);
                        }
                    }
                    // add DataSource;
                    tempDataBuffer.useDataSource = true;
                    tempDataBuffer.dataSource = cleanString(dataSource_);

                    // add data buffer to vector
                    result.push_back(tempDataBuffer);

                }
            } else {
                log << SLevel(ERROR) << "Aborted parsing InfluxDB-Json to databuffer. Unable to find 'columns and/or 'values' in JSON" << endl;
            }
        } else {
            log << SLevel(ERROR) << "Aborted parsing InfluxDB-Json to databuffer. Unable to find 'series' in JSON" << endl;
        }
    }


    return result;
}
