TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS = -std=c++11

LIBS += -lglfw -lGL -lGLEW -lm

SOURCES += src/main.cpp

HEADERS += \
	src/glfwcallbacks.hpp \
	src/glfwadapter.hpp \
	src/tils.hpp \
	src/niform.hpp \
	src/shader.hpp \
	src/simple.hpp \
	src/camera.hpp \
	src/input.hpp \
	src/transform.hpp \
	src/graphics.hpp

OTHER_FILES += \
	shaders/simple.frag \
	shaders/simple.vert

