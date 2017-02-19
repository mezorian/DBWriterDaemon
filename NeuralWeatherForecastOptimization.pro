#-------------------------------------------------
#
# Project created by QtCreator 2016-08-10T17:33:17
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = NeuralWeatherForecastOptimization

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH += include/
INCLUDEPATH += ../include/

LIBS += -lcurl

#DEFINES += CPU_ONLY=1

#INCLUDEPATH += /home/anon/Desktop/CleanMonthly/caffe_repo/caffe/include/
#INCLUDEPATH += /home/anon/Desktop/CleanMonthly/caffe_repo/caffe/distribute/include/
#INCLUDEPATH += include/

#LIBS += -lboost_system
#LIBS += -lglog
#LIBS += -lprotobuf

SOURCES += DBWriterDaemon.cpp \
    src/DataBuffer.cpp \
    src/HTTPRequest.cpp \
    src/OpenWeatherMap.cpp \
    src/WebContent.cpp \
    src/AirPressureSensor.cpp \
    src/DBInterface.cpp \
    src/LogWriter.cpp \
    src/Sensor.cpp \
    src/SLevel.cpp \
    src/TemperatureSensor.cpp \
    test/main.cpp \
    src/Third-Party-Code/DS18B20.cpp \
    test/DBInterfaceTest.cpp \
    test/SensorTest.cpp \
    test/WebContentTest.cpp

HEADERS += \
    include/catch.hpp \
    include/DataBuffer.h \
    include/HTTPRequest.h \
    include/OpenWeatherMap.h \
    include/WebContent.h \
    include/WebContentType.h \
    include/AirPressureSensor.h \
    include/Config.h \
    include/DBInterface.h \
    include/LogWriter.h \
    include/Sensor.h \
    include/SensorType.h \
    include/SLevel.h \
    include/TemperatureSensor.h

#unix:!macx: LIBS += -L$$PWD/../../../../../CleanMonthly/caffe_repo/caffe/build/lib/ -lcaffe

#INCLUDEPATH += $$PWD/../../../../../CleanMonthly/caffe_repo/caffe/build
#DEPENDPATH += $$PWD/../../../../../CleanMonthly/caffe_repo/caffe/build

#unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../CleanMonthly/caffe_repo/caffe/build/lib/libcaffe.a
