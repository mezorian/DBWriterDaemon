/**
 * DBInterfaceTest.cpp
 * Purpose: implements unit-tests for the DBInterface-class
 *
 * @author Wall.Of.Death
 * @version 1.0
 */

#include "DataBuffer.h"
#include "DBInterface.h"

// !!!! TEST_CASE IN COMBINITION WITH REQUIRE IS LIKE A LOOP STRUCTURE !!!!
// !!!! THE TEST_CASE IS EXECUTED FOR EVERY SECTION !!!!

#include "catch.hpp"

// --- Tests for HTTP-REQUEST ---

TEST_CASE( "HTTPRequest.post works") {
    HTTPRequest req;
    string url;
    string postFields;

    SECTION("both parameters valid") {
        url = "http://localhost:8086/write?db=test3";
        postFields = "forecast,datatype=temperature value=50";
        REQUIRE(req.post(url,postFields));
    }

    SECTION("url_ valid, postFields_ empty") {
        url = "http://localhost:8086/query?q=create+database+WeatherData2&db=_internal";
        postFields = "";
        REQUIRE(req.post(url,postFields));
    }

    SECTION("url_ is not valid") {
        url = "http://localhost:86/write?db=test3";
        postFields = "forecast,datatype=temperature value=50";
        REQUIRE_FALSE(req.post(url,postFields));
    }

    SECTION("url_ is empty") {
        url = "";
        postFields = "forecast,datatype=temperature value=50";
        REQUIRE_FALSE(req.post(url,postFields));
    }

}

TEST_CASE( "HTTPRequest.get works") {
    HTTPRequest req;
    string url;

    SECTION("url_ valid") {
        url = "http://localhost:8086/query?pretty=true&db=WeatherData&q=SELECT+Lufttemperatur_2m+FROM+point";
        string result = req.get(url);
        string sub = result.substr(0,20);
        REQUIRE(sub == "{\n    \"results\": [\n ");
    }


    SECTION("url_ is not valid") {
        url = "http://localhost:8086/Lufttemperatur_2m+FROM+point";
        REQUIRE(req.get(url) == "");
    }

    SECTION("url_ is empty") {
        url = "";
        REQUIRE(req.get(url) == "");
    }

}


TEST_CASE("DBInterface, write/read to/from database, getDBFailure") {
    DataBuffer dataBuffer, dataBuffer2;

    DBInterface& dbi = DBInterface::getInstance();
    dbi.init();

    dbi.writeStatusOK(true);

    SECTION ("write / read valid databuffer") {
        //2015-06-11T20:46:02
        dataBuffer.startDateTime.tm_sec  = 2;    // seconds
        dataBuffer.startDateTime.tm_min  = 46;   // minutes
        dataBuffer.startDateTime.tm_hour = 20;   // hours
        dataBuffer.startDateTime.tm_mday = 11;   // day
        dataBuffer.startDateTime.tm_mon  = 6;    // month
        dataBuffer.startDateTime.tm_year = 2015; // Year

        dataBuffer.endDateTime = dataBuffer.startDateTime;
        dataBuffer.useDateTimes = true;

        dataBuffer.dataSource = "Forecast";
        dataBuffer.useDataSource = true;

        dataBuffer.data["Lufttemperatur_2m"] = 14.5;
        dataBuffer.data["RelativeLuftfeuchte_2m"] = 1200;
        dbi.writeToDataBase(dataBuffer);

        dataBuffer2 = dataBuffer;
        dataBuffer2.data["Lufttemperatur_2m"] = 0;
        dataBuffer2.data["RelativeLuftfeuchte_2m"] = 0;
        vector<DataBuffer> dataBufferVec = dbi.readFromDataBase(dataBuffer2);
        REQUIRE(dataBufferVec[0] == dataBuffer);
        REQUIRE(dbi.getDBFailure() == false);
    }

    SECTION ("write / read date = 01.01.1972") {
        dataBuffer.startDateTime.tm_sec  = 0;   // seconds
        dataBuffer.startDateTime.tm_min  = 0;   // minutes
        dataBuffer.startDateTime.tm_hour = 0;   // hours
        dataBuffer.startDateTime.tm_mday = 1;   // day
        dataBuffer.startDateTime.tm_mon  = 1;    // month
        dataBuffer.startDateTime.tm_year = 1972; // Year

        dataBuffer.endDateTime = dataBuffer.startDateTime;
        dataBuffer.useDateTimes = true;

        dataBuffer.dataSource = "Forecast";
        dataBuffer.useDataSource = true;

        dataBuffer.data["Lufttemperatur_2m"] = 14.5;
        dataBuffer.data["RelativeLuftfeuchte_2m"] = 1200;
        dbi.writeToDataBase(dataBuffer);

        dataBuffer2 = dataBuffer;
        dataBuffer2.data["Lufttemperatur_2m"] = 0;
        dataBuffer2.data["RelativeLuftfeuchte_2m"] = 0;
        vector<DataBuffer> dataBufferVec = dbi.readFromDataBase(dataBuffer2);
        REQUIRE(dataBufferVec[0] == dataBuffer);
        REQUIRE(dbi.getDBFailure() == false);
    }

    SECTION ("write / read date < 01.01.1972") {
        dataBuffer.startDateTime.tm_sec  = 0;   // seconds
        dataBuffer.startDateTime.tm_min  = 0;   // minutes
        dataBuffer.startDateTime.tm_hour = 0;   // hours
        dataBuffer.startDateTime.tm_mday = 31;   // day
        dataBuffer.startDateTime.tm_mon  = 12;    // month
        dataBuffer.startDateTime.tm_year = 1971; // Year

        dataBuffer.endDateTime = dataBuffer.startDateTime;
        dataBuffer.useDateTimes = true;

        dataBuffer.dataSource = "Forecast";
        dataBuffer.useDataSource = true;

        dataBuffer.data["Lufttemperatur_2m"] = 14.5;
        dataBuffer.data["RelativeLuftfeuchte_2m"] = 1200;
        dbi.writeToDataBase(dataBuffer);

        dataBuffer2 = dataBuffer;
        dataBuffer2.data["Lufttemperatur_2m"] = 0;
        dataBuffer2.data["RelativeLuftfeuchte_2m"] = 0;
        vector<DataBuffer> dataBufferVec = dbi.readFromDataBase(dataBuffer2);
        REQUIRE(dataBufferVec.size() == 0);
        REQUIRE(dbi.getDBFailure() == true);
    }

    /* TODO Influx Error

    SECTION ("write / read with current local time") {
        dataBuffer.useDateTimes = false;

        dataBuffer.dataSource = "Forecast";
        dataBuffer.useDataSource = true;

        dataBuffer.data["Lufttemperatur_2m"] = -10.5;
        dataBuffer.data["RelativeLuftfeuchte_2m"] = 90.4;
        dataBuffer.data["Windgeschwindigkeit_Stundenmittel"] = 10.345;

        dbi.writeToDataBase(dataBuffer);

        dataBuffer2 = dataBuffer;
        dataBuffer2.data["Lufttemperatur_2m"] = 0;
        dataBuffer2.data["RelativeLuftfeuchte_2m"] = 0;
        dataBuffer2.data["Windgeschwindigkeit_Stundenmittel"] = 0;
        vector<DataBuffer> dataBufferVec = dbi.readFromDataBase(dataBuffer2);
        REQUIRE(dataBufferVec[0] == dataBuffer);
        REQUIRE(dbi.getDBFailure() == false);
    }*/


    SECTION ("write / read maximum / minimum values") {
        dataBuffer.startDateTime.tm_sec  = 0;   // seconds
        dataBuffer.startDateTime.tm_min  = 0;   // minutes
        dataBuffer.startDateTime.tm_hour = 0;   // hours
        dataBuffer.startDateTime.tm_mday = 31;   // day
        dataBuffer.startDateTime.tm_mon  = 12;   // month
        dataBuffer.startDateTime.tm_year = 1972; // Year

        dataBuffer.endDateTime = dataBuffer.startDateTime;
        dataBuffer.useDateTimes = true;

        dataBuffer.dataSource = "Forecast";
        dataBuffer.useDataSource = true;

        dataBuffer.data["Lufttemperatur_2m"] = INFLUXDB_MAX;
        dataBuffer.data["RelativeLuftfeuchte_2m"] = INFLUXDB_MIN;
        dataBuffer.data["Windgeschwindigkeit_Stundenmittel"] = 0;

        dbi.writeToDataBase(dataBuffer);

        dataBuffer2 = dataBuffer;
        dataBuffer2.data["Lufttemperatur_2m"] = 0;
        dataBuffer2.data["RelativeLuftfeuchte_2m"] = 0;
        dataBuffer2.data["Windgeschwindigkeit_Stundenmittel"] = 0;
        vector<DataBuffer> dataBufferVec = dbi.readFromDataBase(dataBuffer2);
        REQUIRE(dataBufferVec[0] == dataBuffer);
        REQUIRE(dbi.getDBFailure() == false);
    }

    SECTION ("write / read invalid values and strings") {
        // invalid values get cut to range
        // invalid extra signs get removed
        // (alphabetical chars, numbers and '_' are allowed)
        dataBuffer.startDateTime.tm_sec  = 59;   // seconds
        dataBuffer.startDateTime.tm_min  = 59;   // minutes
        dataBuffer.startDateTime.tm_hour = 1;   // hours
        dataBuffer.startDateTime.tm_mday = 10;   // day
        dataBuffer.startDateTime.tm_mon  = 8;   // month
        dataBuffer.startDateTime.tm_year = 2016; // Year

        dataBuffer.endDateTime = dataBuffer.startDateTime;
        dataBuffer.useDateTimes = true;

        dataBuffer.dataSource = "Foreca%st";
        dataBuffer.useDataSource = true;

        dataBuffer.data["Luft%&te2mperatur_2m"] = INFLUXDB_MAX+1;
        dataBuffer.data["RelativeL34uf'tfeuchte_2m"] = INFLUXDB_MIN-1;
        dataBuffer.data["Windgeschwindi()gk?eit_Stundenmittel"] = 0;

        dbi.writeToDataBase(dataBuffer);

        dataBuffer2 = dataBuffer;
        dataBuffer2.data["Luft%&te2mperatur_2m"] = 0;
        dataBuffer2.data["RelativeL34uf'tfeuchte_2m"] = 0;
        dataBuffer2.data["Windgeschwindi()gk?eit_Stundenmittel"] = 0;
        vector<DataBuffer> dataBufferVec = dbi.readFromDataBase(dataBuffer2);

        DataBuffer dataBufferWithoutExtraSigns;

        dataBufferWithoutExtraSigns.startDateTime = dataBuffer.startDateTime;
        dataBufferWithoutExtraSigns.endDateTime   = dataBuffer.endDateTime;
        dataBufferWithoutExtraSigns.useDateTimes = true;

        dataBufferWithoutExtraSigns.dataSource = "Forecast";
        dataBufferWithoutExtraSigns.useDataSource = true;

        dataBufferWithoutExtraSigns.data["Luftte2mperatur_2m"] = INFLUXDB_MAX+1;
        dataBufferWithoutExtraSigns.data["RelativeL34uftfeuchte_2m"] = INFLUXDB_MIN-1;
        dataBufferWithoutExtraSigns.data["Windgeschwindigkeit_Stundenmittel"] = 0;

        REQUIRE(dataBufferVec[0] == dataBufferWithoutExtraSigns);
        REQUIRE(dbi.getDBFailure() == false);
    }

}

TEST_CASE ("Write / Read Status") {
    DBInterface& dbi = DBInterface::getInstance();
    dbi.init();

    SECTION("Write/Read of true works") {
        dbi.writeStatusOK(true);
        REQUIRE(dbi.readStatusOK());
    }

    SECTION("Write/Read of false works") {
        dbi.writeStatusOK(false);
        REQUIRE_FALSE(dbi.readStatusOK());
    }
}
