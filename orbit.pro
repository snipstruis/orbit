TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS = -std=c++11

LIBS += -lglfw -lGL -lGLEW -lm

INCLUDEPATH += ../matrix/

SOURCES += main.cpp

HEADERS += \
	glfwcallbacks.hpp \
	glfwadapter.hpp \
	utils.hpp \
	uniform.hpp \
    shader.hpp \
    simple.hpp \
    camera.hpp \
    input.hpp

OTHER_FILES += \
    simple.frag \
    simple.vert

