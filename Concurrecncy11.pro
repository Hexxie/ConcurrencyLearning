TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11 -pthread
LIBS += -Wl,--no-as-needed -lpthread


SOURCES += \
    examples/basicConcurrency.cpp \
    examples/detachexample.cpp \
    examples/thread_guard.cpp \
    examples/alarm_mutex/alarm_mutex.c \
    puzzles/basic/barrier.c \
    puzzles/basic/multiplex.c \
    puzzles/basic/mutex.c \
    puzzles/basic/queue.c \
    puzzles/basic/rendezvous.c \
    puzzles/basic/reusable_barrier.c \
    examples/trylock.c \
    puzzles/basic/barrier.c \
    puzzles/other/semaphoreUsage.c

HEADERS += \
    examples/thread_guard.h
