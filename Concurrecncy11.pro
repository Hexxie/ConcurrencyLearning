TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11 -pthread
LIBS += -Wl,--no-as-needed -lpthread


SOURCES += \
    basicConcurrency.cpp \
    thread_guard.cpp \
    detachexample.cpp

HEADERS +=
