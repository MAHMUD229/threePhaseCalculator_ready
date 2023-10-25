QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_test_writecurrenttofile.cpp \
            functions.cpp

HEADERS += \
    tst_test_writecurrenttofile.h \
    functions.h
