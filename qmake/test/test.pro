include(../global.pri)
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = test

QMAKE_CXXFLAGS = -std=c++11

LIBS += -lglfw -lGL -lGLEW -lm -ljsoncpp
LIBS += -L $$ROOT/qmake/lib/ -lorbit
LIBS += -lunittest++

SOURCES += $$ROOT/tests/test.cpp

HEADERS += $$ROOT/tests/*.hpp
