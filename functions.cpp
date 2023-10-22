#include "functions.h"


QMap<QString, Phase> parseXml(const QString &filePath, long double &voltage, QString &circuitType) {
    QMap<QString, Phase> phasesMap;

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file.";
        return phasesMap;
    }

    QXmlStreamReader xmlReader(&file);

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        if (token == QXmlStreamReader::StartElement) {
            QString elementName = xmlReader.name().toString();

            if (elementName == "Circuit") {
                QString voltageString = xmlReader.attributes().value("voltage").toString();
                bool ok;
                voltage = voltageString.toDouble(&ok);

                if (!ok) {
                    qDebug() << "Error: circuit must contain Voltage.";
                    file.close();
                    return phasesMap;
                }

                if (voltage <= 0) {
                    qDebug() << "Error: Voltage must be a positive value.";
                    file.close();
                    return phasesMap;
                }

                circuitType = xmlReader.attributes().value("type").toString();

                if (circuitType.toLower() != "star" && circuitType.toLower() != "triangle") {
                    qDebug() << "Error: Unknown circuit type: " << circuitType;
                    file.close();
                    return phasesMap;
                }
            }

            if (elementName == "Phase") {
                QString phaseName = xmlReader.attributes().value("name").toString();

                if (!phasesMap.contains(phaseName)) {
                    phasesMap[phaseName] = Phase();
                }

                Phase &phase = phasesMap[phaseName];

                bool phaseHasComponents = false;

                bool phaseHasNonZeroComponent = false;

                while (!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "Phase")) {
                    if (xmlReader.tokenType() == QXmlStreamReader::StartElement) {
                        QString elementType = xmlReader.name().toString();

                        if (elementType == "Resistor" || elementType == "Inductor" || elementType == "Capacitor") {
                            phaseHasComponents = true;

                            Component component;

                            QXmlStreamAttributes componentAttributes = xmlReader.attributes();
                            component.name = componentAttributes.value("name").toString();
                            QString resistanceString = componentAttributes.value("resistance").toString();
                            bool ok;
                            component.resistance = resistanceString.toDouble(&ok);

                            if (!ok) {
                                qDebug() << "Error converting string to number: " << resistanceString;
                                xmlReader.readNext();
                                continue;
                            }

                            if (component.resistance < 0) {
                                qDebug() << "Skipping component " << component.name << " due to negative resistance.";
                                xmlReader.readNext();
                                continue;
                            }

                            if (component.resistance != 0) {
                                phaseHasNonZeroComponent = true;
                            }

                            if (elementType == "Resistor") {
                                phase.resistors.append(component);
                            } else if (elementType == "Inductor") {
                                phase.inductors.append(component);
                            } else if (elementType == "Capacitor") {
                                phase.capacitors.append(component);
                            }
                        }
                    }

                    xmlReader.readNext();
                }

                if (!phaseHasComponents || !phaseHasNonZeroComponent) {
                    qDebug() << "Skipping phase " << phaseName << " due to absence of components or all components have zero resistance.";
                    phasesMap.remove(phaseName);
                }

            }
        }
    }

    if (xmlReader.hasError()) {
        qDebug() << "XML parsing error: " << xmlReader.errorString();
    } else {
        qDebug() << "XML file parsed succesfully!";
    }

    file.close();

    return phasesMap;
}



QMap<QString, long double> calculateCurrent(const QMap<QString, Phase> &phasesMap, long double voltage, const QString &circuitType) {
    QMap<QString, long double> currentMap;

    foreach (const QString &phaseName, phasesMap.keys()) {
        const Phase &phase = phasesMap[phaseName];

        long double totalResistance = 0.0;
        long double totalInductiveReactance = 0.0;
        long double totalCapacitiveReactance = 0.0;


        foreach (const Component &resistor, phase.resistors) {
            totalResistance += resistor.resistance;
        }

        foreach (const Component &inductor, phase.inductors) {
            totalInductiveReactance += 2 * M_PI * inductor.resistance;
        }

        foreach (const Component &capacitor, phase.capacitors) {
            if(capacitor.resistance != 0){
                totalCapacitiveReactance += 1 / (2 * M_PI * capacitor.resistance);
            }
        }

        long double totalImpedance = sqrt(pow(totalResistance, 2) + pow((totalInductiveReactance - totalCapacitiveReactance), 2));
        long double phaseVoltage = 0.0;
        long double current = 0.0;

        if (circuitType.toLower() == "star") {// Звезда
            phaseVoltage = voltage / sqrt(3);
            current = phaseVoltage / totalImpedance;
        } else if (circuitType.toLower() == "triangle") {// Треугольник
            phaseVoltage = voltage;
            current = phaseVoltage / totalImpedance;
        }

        currentMap[phaseName] = current;
    }

    if (!currentMap.isEmpty()){
        qDebug() << "Current calculated succesfully!";
    }

    return currentMap;
}



void writeCurrentToFile(const QMap<QString, long double> &currentMap, const QString &outputFilePath) {
    QFile outputFile(outputFilePath);

    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file for writing.";
        return;
    }

    QTextStream stream(&outputFile);

    if (currentMap.isEmpty()) {
        qDebug() << "Failed to write current due to empty current Map.";
        return;
    }

    foreach (const QString &phaseName, currentMap.keys()) {
        long double current = currentMap[phaseName];

        stream << phaseName << " : " << QString::number(current, 'g', 4) << " A\n";
    }

    outputFile.close();
}
