QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrafficSimulation
TEMPLATE = app


SOURCES += \
    src/businessman.cpp \
    src/dialog.cpp \
    src/gridsquare.cpp \
    src/main.cpp \
    src/pathnode.cpp \
    src/person.cpp \
    src/simulationcontroller.cpp \
    src/tourist.cpp \
    src/trafficlight.cpp \
    src/train.cpp

HEADERS  += \
    src/businessman.h \
    src/dialog.h \
    src/globals.h \
    src/gridsquare.h \
    src/pathnode.h \
    src/person.h \
    src/simulationcontroller.h \
    src/tourist.h \
    src/trafficlight.h \
    src/train.h

FORMS    += \
    src/dialog.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    example path.xlsx
