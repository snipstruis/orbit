TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS = -std=c++11

LIBS += -lglfw -lGL -lGLEW -lm -ljsoncpp

SOURCES += src/main.cpp

HEADERS += \
	src/core/*		\
	src/graphics/*	\
	src/input/*		\
	src/utils/*

INCLUDEPATH += src/

OTHER_FILES += \
	shaders/*
	meshes/*
	scenes/*

