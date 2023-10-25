#ifndef TST_TEST_PARSEXML_H
#define TST_TEST_PARSEXML_H

#include <QtTest/QtTest>
#include <QCoreApplication>
#include "functions.h"


class TestParseXml : public QObject {
    Q_OBJECT

private slots:
    void testEmptyFile() {
        QString filePath = "..\\parseXml\\input_files\\emptyFile.xml";
        long double voltage;
        QString circuitType;
        auto result = parseXml(filePath, voltage, circuitType);

        QVERIFY(result.isEmpty());
        QVERIFY2(result.isEmpty(), "Failed to handle invalid file.");

        QMapIterator<QString, Phase> it(result);
            while (it.hasNext()) {
                it.next();

                qDebug() << it.key();

                // Вывод информации о резисторах
                for (const Component &resistor : it.value().resistors) {
                    qDebug() << resistor.name << QString::number(resistor.resistance, 'g', 4);
                }

                // Вывод информации об индукторах
                for (const Component &inductor : it.value().inductors) {
                    qDebug() << inductor.name << QString::number(inductor.resistance, 'g', 4);
                }

                // Вывод информации о конденсаторах
                for (const Component &capacitor : it.value().capacitors) {
                    qDebug() << capacitor.name << QString::number(capacitor.resistance, 'g', 4);
                }

                qDebug() << "";
            }
    }


    void testValidInput() {
        QString filePath = "..\\parseXml\\input_files\\normalData.xml";
        long double voltage;
        QString circuitType;
        auto result = parseXml(filePath, voltage, circuitType);

        QVERIFY2(!result.isEmpty(), "Failed to handle normal data.");

        QMapIterator<QString, Phase> it(result);
            while (it.hasNext()) {
                it.next();

                qDebug() << it.key();

                // Вывод информации о резисторах
                for (const Component &resistor : it.value().resistors) {
                    qDebug() << resistor.name << QString::number(resistor.resistance, 'g', 4);
                }

                // Вывод информации об индукторах
                for (const Component &inductor : it.value().inductors) {
                    qDebug() << inductor.name << QString::number(inductor.resistance, 'g', 4);
                }

                // Вывод информации о конденсаторах
                for (const Component &capacitor : it.value().capacitors) {
                    qDebug() << capacitor.name << QString::number(capacitor.resistance, 'g', 4);
                }

                qDebug() << "";
            }

    }


    void testNonTypicalCircuitType() {
        QString filePath = "..\\parseXml\\input_files\\nonTypicalCircuit.xml";
        long double voltage;
        QString circuitType;
        auto result = parseXml(filePath, voltage, circuitType);

        QVERIFY2(result.isEmpty(), "Failed to handle non-typical circuit type.");

        QMapIterator<QString, Phase> it(result);
            while (it.hasNext()) {
                it.next();

                qDebug() << it.key();

                // Вывод информации о резисторах
                for (const Component &resistor : it.value().resistors) {
                    qDebug() << resistor.name << QString::number(resistor.resistance, 'g', 4);
                }

                // Вывод информации об индукторах
                for (const Component &inductor : it.value().inductors) {
                    qDebug() << inductor.name << QString::number(inductor.resistance, 'g', 4);
                }

                // Вывод информации о конденсаторах
                for (const Component &capacitor : it.value().capacitors) {
                    qDebug() << capacitor.name << QString::number(capacitor.resistance, 'g', 4);
                }

                qDebug() << "";
            }
    }


    void testNonVoltage() {
        QString filePath = "..\\parseXml\\input_files\\nonVoltageCircuit.xml";
        long double voltage;
        QString circuitType;
        auto result = parseXml(filePath, voltage, circuitType);

        QVERIFY2(result.isEmpty(), "Failed to handle non-voltage circuit.");

        QMapIterator<QString, Phase> it(result);
            while (it.hasNext()) {
                it.next();

                qDebug() << it.key();

                // Вывод информации о резисторах
                for (const Component &resistor : it.value().resistors) {
                    qDebug() << resistor.name << QString::number(resistor.resistance, 'g', 4);
                }

                // Вывод информации об индукторах
                for (const Component &inductor : it.value().inductors) {
                    qDebug() << inductor.name << QString::number(inductor.resistance, 'g', 4);
                }

                // Вывод информации о конденсаторах
                for (const Component &capacitor : it.value().capacitors) {
                    qDebug() << capacitor.name << QString::number(capacitor.resistance, 'g', 4);
                }

                qDebug() << "";
            }
    }


    void testNegativeVoltage() {
        QString filePath = "..\\parseXml\\input_files\\negativeVoltage.xml";
        long double voltage;
        QString circuitType;
        auto result = parseXml(filePath, voltage, circuitType);

        QVERIFY2(result.isEmpty(), "Failed to handle negative voltage circuit.");

        QMapIterator<QString, Phase> it(result);
            while (it.hasNext()) {
                it.next();

                qDebug() << it.key();

                // Вывод информации о резисторах
                for (const Component &resistor : it.value().resistors) {
                    qDebug() << resistor.name << QString::number(resistor.resistance, 'g', 4);
                }

                // Вывод информации об индукторах
                for (const Component &inductor : it.value().inductors) {
                    qDebug() << inductor.name << QString::number(inductor.resistance, 'g', 4);
                }

                // Вывод информации о конденсаторах
                for (const Component &capacitor : it.value().capacitors) {
                    qDebug() << capacitor.name << QString::number(capacitor.resistance, 'g', 4);
                }

                qDebug() << "";
            }
    }


    void testNegativeResistance() {
        QString filePath = "..\\parseXml\\input_files\\negativeResistance.xml";
        long double voltage;
        QString circuitType;
        auto result = parseXml(filePath, voltage, circuitType);

        QVERIFY2(!result.isEmpty(), "Failed to handle negative resistance circuit.");

        QMapIterator<QString, Phase> it(result);
            while (it.hasNext()) {
                it.next();

                qDebug() << it.key();

                // Вывод информации о резисторах
                for (const Component &resistor : it.value().resistors) {
                    qDebug() << resistor.name << QString::number(resistor.resistance, 'g', 4);
                }

                // Вывод информации об индукторах
                for (const Component &inductor : it.value().inductors) {
                    qDebug() << inductor.name << QString::number(inductor.resistance, 'g', 4);
                }

                // Вывод информации о конденсаторах
                for (const Component &capacitor : it.value().capacitors) {
                    qDebug() << capacitor.name << QString::number(capacitor.resistance, 'g', 4);
                }

                qDebug() << "";
            }
    }


    void testZeroComponents() {
        QString filePath = "..\\parseXml\\input_files\\zeroComponents.xml";
        long double voltage;
        QString circuitType;
        auto result = parseXml(filePath, voltage, circuitType);

        QVERIFY2(result.isEmpty(), "Failed to handle zero components circuit.");

        QMapIterator<QString, Phase> it(result);
            while (it.hasNext()) {
                it.next();

                qDebug() << it.key();

                // Вывод информации о резисторах
                for (const Component &resistor : it.value().resistors) {
                    qDebug() << resistor.name << QString::number(resistor.resistance, 'g', 4);
                }

                // Вывод информации об индукторах
                for (const Component &inductor : it.value().inductors) {
                    qDebug() << inductor.name << QString::number(inductor.resistance, 'g', 4);
                }

                // Вывод информации о конденсаторах
                for (const Component &capacitor : it.value().capacitors) {
                    qDebug() << capacitor.name << QString::number(capacitor.resistance, 'g', 4);
                }

                qDebug() << "";
            }
    }


    void testNonComponents() {
        QString filePath = "..\\parseXml\\input_files\\nonComponents.xml";
        long double voltage;
        QString circuitType;
        auto result = parseXml(filePath, voltage, circuitType);

        QVERIFY2(result.isEmpty(), "Failed to handle non-components circuit.");

        QMapIterator<QString, Phase> it(result);
            while (it.hasNext()) {
                it.next();

                qDebug() << it.key();

                // Вывод информации о резисторах
                for (const Component &resistor : it.value().resistors) {
                    qDebug() << resistor.name << QString::number(resistor.resistance, 'g', 4);
                }

                // Вывод информации об индукторах
                for (const Component &inductor : it.value().inductors) {
                    qDebug() << inductor.name << QString::number(inductor.resistance, 'g', 4);
                }

                // Вывод информации о конденсаторах
                for (const Component &capacitor : it.value().capacitors) {
                    qDebug() << capacitor.name << QString::number(capacitor.resistance, 'g', 4);
                }

                qDebug() << "";
            }
    }


    void testOneComponent() {
        QString filePath = "..\\parseXml\\input_files\\oneElementPhase.xml";
        long double voltage;
        QString circuitType;
        auto result = parseXml(filePath, voltage, circuitType);

        QVERIFY2(!result.isEmpty(), "Failed to handle one component in phase.");

        QMapIterator<QString, Phase> it(result);
            while (it.hasNext()) {
                it.next();

                qDebug() << it.key();

                // Вывод информации о резисторах
                for (const Component &resistor : it.value().resistors) {
                    qDebug() << resistor.name << QString::number(resistor.resistance, 'g', 4);
                }

                // Вывод информации об индукторах
                for (const Component &inductor : it.value().inductors) {
                    qDebug() << inductor.name << QString::number(inductor.resistance, 'g', 4);
                }

                // Вывод информации о конденсаторах
                for (const Component &capacitor : it.value().capacitors) {
                    qDebug() << capacitor.name << QString::number(capacitor.resistance, 'g', 4);
                }

                qDebug() << "";
            }
    }

private:

};


#endif // TST_TEST_PARSEXML_H
