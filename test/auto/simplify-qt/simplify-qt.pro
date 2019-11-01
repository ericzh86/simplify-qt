QT += testlib
QT -= gui

TEMPLATE = app

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

HEADERS += \
    simplifytest.h
SOURCES += \
    simplifytest.cpp

include(../../../src/simplify-qt/simplify-qt.pri)
