/**
 * DBInterface.h
 * Purpose: defines class DBInterface
 *
 * @author Wall.Of.Death
 * @version 1.0 20160704
 */

#ifndef DBINTERFACE_H
#define DBINTERFACE_H

// todo
#include <QStringList>
// --
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
// ---
#include <sstream>
#include <algorithm>
#include <iomanip>
// ---
#include "DataBuffer.h"
#include "Config.h"
#include "LogWriter.h"
#include "HTTPRequest.h"

/**
 * DBInterface class
 * @brief DBInterface provides access to the database
 * The DBInterface class provides acces to the database by having public functions for
 * initializing, reading and writing data to the database.
 * It is designed by the design pattern singleton.
 */
class DBInterface {
    public:
        /* --- needed to create a singleton --- */
        static DBInterface& getInstance() {
            static DBInterface _instance;
            return _instance;
        }
        ~DBInterface() {}

        /* --- interfacing the database --- */
        void               init             ();
        void               writeToDataBase  (DataBuffer &dataBuffer_);
        vector<DataBuffer> readFromDataBase (DataBuffer &dataBuffer_);
        void               writeStatusOK    (      bool  statusOK_  );
        bool               readStatusOK     ();
        bool               getDBFailure     ();


    private:
        /* --- needed to create a singleton --- */
        DBInterface() {};                  // must be private to avoid the creation of DBInterface from the outside by the constructor
                                           // use 'protected' if you want to inherit from this class
        DBInterface            (const DBInterface& dbInterface_); // must be private to avoid creation of another DBInterface-instance by copy-constructor
        DBInterface& operator= (const DBInterface& dbInterface_); // must be private to avoid creating an new DBInterface-instance by copying

        /* --- interfacing the database --- */
        void createIfNotCreatedDataBase();
        void setDBFailure(bool val_) {dbFailure = val_;} ;
        bool dbFailure = false;
        LogWriter log;

        /* --- miscellaneous --- */
        string cleanString(const string& stringToClean_);
        double cutValueToInfluxDBRange(double val_);
        string deletePaddingZeros(const string& stringWithPaddingZeros_);
        string cTimeToString(struct tm datetime_, bool inUnixTime_);
        struct tm stringToCTime(const string& dateTimeString_);
        struct tm getCurrentDateTime(bool cutToHours_ = true);
        int getCurrentDateTimeAsUnixTime(bool cutToHours_ = true);
        vector<DataBuffer> jsonToDataBufferVector(const string& json_, const string &dataSource_);
};

#endif // DBINTERFACE_H
