#ifndef TST_TEST_CALCULATECURRENT_H
#define TST_TEST_CALCULATECURRENT_H


#include <QtTest/QtTest>
#include <QCoreApplication>
#include "functions.h"


class test_calculateCurrent : public QObject
{
    Q_OBJECT

private slots:
    void testNonCircuitMap() {
        QString filePath = "..\\calculateCurrent\\input_files\\emptyFile.xml";
        long double voltage;
        QString circuitType;
        auto map = parseXml(filePath, voltage, circuitType);

        auto result = calculateCurrent(map, voltage, circuitType);

        QVERIFY2(result.isEmpty(), "Failed to handle no circuit map.");

        QMapIterator<QString, long double> it(result);
            while (it.hasNext()) {
                it.next();

                qDebug() << it.key();
                qDebug() << QString::number(it.value(), 'g', 4);
                qDebug() << " ";
            }
    }


    void testNormalData() {
        QString filePath = "..\\calculateCurrent\\input_files\\normalData.xml";
        long double voltage;
        QString circuitType;
        auto map = parseXml(filePath, voltage, circuitType);

        auto result = calculateCurrent(map, voltage, circuitType);

        QVERIFY2(!result.isEmpty(), "Failed to handle normal data.");

        QMapIterator<QString, long double> it(result);
            while (it.hasNext()) {
                it.next();

                qDebug() << it.key();
                qDebug() << QString::number(it.value(), 'g', 4);
                qDebug() << " ";
            }
    }

};


#endif // TST_TEST_CALCULATECURRENT_H
