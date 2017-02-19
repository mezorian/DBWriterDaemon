/**
 * WebContentTest.cpp
 * Purpose: implements unit-tests for all WebContent-classes
 *
 * @author Wall.Of.Death
 * @version 1.0
 */

#include "DataBuffer.h"
#include "DBInterface.h"
#include "WebContent.h"
#include "OpenWeatherMap.h"

// !!!! TEST_CASE IN COMBINITION WITH REQUIRE IS LIKE A LOOP STRUCTURE !!!!
// !!!! THE TEST_CASE IS EXECUTED FOR EVERY SECTION !!!!

#include "catch.hpp"

// --- Tests for WebContent

TEST_CASE("readWebContent works") {
    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    dbi.init();
    // todo
    dbi.writeStatusOK(true);

    SECTION("readWebContent works for OpenWeatherMap") {
        WebContent WebContent1;
        WebContent1.setWebContentType(new OpenWeatherMap());
        DataBuffer openWeatherMapBuffer = WebContent1.readWebContent();
        REQUIRE(openWeatherMapBuffer.data.size() > 0);
        REQUIRE(openWeatherMapBuffer.data["Lufttemperatur_2m"] > -2000);
        REQUIRE(openWeatherMapBuffer.data["Lufttemperatur_2m"] <  2000);
        REQUIRE(openWeatherMapBuffer.data["Luftdruck_2m"] > -2000);
        REQUIRE(openWeatherMapBuffer.data["Luftdruck_2m"] <  2000);
        REQUIRE(openWeatherMapBuffer.data["RelativeLuftfeuchte_2m"] > -2000);
        REQUIRE(openWeatherMapBuffer.data["RelativeLuftfeuchte_2m"] <  2000);
        REQUIRE(openWeatherMapBuffer.data["Windgeschwindigkeit"] > -2000);
        REQUIRE(openWeatherMapBuffer.data["Windgeschwindigkeit"] <  2000);
        REQUIRE(openWeatherMapBuffer.data["Windrichtung"] > -2000);
        REQUIRE(openWeatherMapBuffer.data["Windrichtung"] <  2000);
        REQUIRE(dbi.readStatusOK());
    }
}
