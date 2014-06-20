include(../global.pri)
CONFIG -= qt
CONFIG += staticlib

TARGET = orbit
TEMPLATE = lib

LIBS += -lglfw -lGL -lGLEW -lm -ljsoncpp

HEADERS += \
	$$ROOT/src/core/*     \
	$$ROOT/src/graphics/* \
	$$ROOT/src/input/*    \
	$$ROOT/src/utils/*
