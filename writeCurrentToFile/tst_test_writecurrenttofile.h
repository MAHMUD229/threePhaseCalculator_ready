#ifndef TST_TEST_WRITECURRENTTOFILE_H
#define TST_TEST_WRITECURRENTTOFILE_H


#include <QtTest>
#include <QCoreApplication>
#include "functions.h"

class test_writeCurrentToFile : public QObject
{
    Q_OBJECT


private slots:
    void testNonCircuitMap() {
        QString filePath = "..\\writeCurrentToFile\\input_files\\emptyFile.xml";
        QString outputFilePath = "..\\writeCurrentToFile\\output_files\\output.txt";
        long double voltage;
        QString circuitType;
        auto map = parseXml(filePath, voltage, circuitType);

        auto result = calculateCurrent(map, voltage, circuitType);

        try {
            writeCurrentToFile(result, outputFilePath);
        } catch (const QString &errorMessage) {
            qDebug() << "Error: " << errorMessage;
        } catch (...) {
            qDebug() << "An unknown error occurred.";
        }
    }


    void testNormalData() {
        QString filePath = "..\\writeCurrentToFile\\input_files\\normalData.xml";
        QString outputFilePath = "..\\writeCurrentToFile\\output_files\\output.txt";
        long double voltage;
        QString circuitType;
        auto map = parseXml(filePath, voltage, circuitType);

        auto result = calculateCurrent(map, voltage, circuitType);

        try {
            writeCurrentToFile(result, outputFilePath);
        } catch (const QString &errorMessage) {
            qDebug() << "Error: " << errorMessage;
        } catch (...) {
            qDebug() << "An unknown error occurred.";
        }
    }
};


#endif // TST_TEST_WRITECURRENTTOFILE_H
