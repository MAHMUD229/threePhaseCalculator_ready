QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_calculatecurrent.cpp \
    functions.cpp

HEADERS += \
    functions.h \
    tst_test_calculatecurrent.h
