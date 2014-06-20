include(../global.pri)
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = $$ROOT/app

QMAKE_CXXFLAGS = -std=c++11

LIBS += -lglfw -lGL -lGLEW -lm -ljsoncpp
LIBS += -L $$ROOT/qmake/lib/ -lorbit

SOURCES += $$ROOT/src/main.cpp

HEADERS += \
	$$ROOT/src/core/*     \
	$$ROOT/src/graphics/* \
	$$ROOT/src/input/*    \
	$$ROOT/src/utils/*

OTHER_FILES += \
	$$ROOT/data/shaders/* \
	$$ROOT/data/meshes/*  \
	$$ROOT/data/scenes/*

