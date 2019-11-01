QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG -= c++11
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    main.cpp

HEADERS += \
    MainWindow.h
SOURCES += \
    MainWindow.cpp

include(../../src/simplify-qt/simplify-qt.pri)
