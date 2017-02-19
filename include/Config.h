/**
 * DataBuffer.h
 * Purpose: defines constants, enums and structs
 *
 * @author Wall.Of.Death
 * @version 1.0 20160704
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <float.h>
#include <QDateTime>

using namespace std;

/* --- constants --- */

// general purpose
const string URL_OF_OPEN_WEATHER_MAP = "http://api.openweathermap.org/data/2.5/forecast?lat=52.9&lon=9.2&APPID=08854a5d6fe0754f2670f5fa5127a831";
const string URL_OF_DATABASE = "http://localhost:8086";
const string NAME_OF_DATBASE = "WeatherData";
const string PATH_OF_LOGFILE = "LogFile.txt";

// training - caffe
const string PATH_OF_NET_STRUCTURE_WITHOUT_LOSS = "../NeuralWeatherForecastOptimization/prototxt/40_12_4_1_RELU_without_loss.prototxt";
const string PATH_OF_TRAINED_WEIGHTS            = "train_iter_50000.caffemodel";
const string PATH_OF_NET_SOLVER                 = "../NeuralWeatherForecastOptimization/prototxt/40_12_4_1_RELU_ADAGRADsolver.prototxt";

// training ann-wrapper
const double    PROPORTION_OF_TRAINING_SET       = 0.8;
const QDateTime START_DATE_TIME_OF_TRAINING_SET  = QDateTime(QDate(2014, 8, 19), QTime( 0, 0, 0));
const QDateTime END_DATE_TIME_OF_TRAINING_SET    = QDateTime(QDate(2016, 7, 16), QTime(21, 0, 0));
const int       TOTAL_NUMBER_OF_TRAINING_SAMPLES = (START_DATE_TIME_OF_TRAINING_SET.secsTo(END_DATE_TIME_OF_TRAINING_SET) / 3600) + 1;
const int       DISTANCE_OF_PREDICTION           = 24;
const int       SCALING_FACTOR                   = 4;
const double    OFFSET                           = 5.0;

// database
const double INFLUXDB_MAX    =  1.79769e+308;
const double INFLUXDB_MIN    = -1.79769e+308;

// hardware / sensors
const int    PIN_OF_DS18B20  = 21;

#endif // CONFIG_H
