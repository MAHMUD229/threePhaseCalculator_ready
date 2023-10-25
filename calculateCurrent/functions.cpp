#include "functions.h"


QMap<QString, Phase> parseXml(const QString &filePath, long double &voltage, QString &circuitType) {
    QMap<QString, Phase> phasesMap;  // Карта фаз

    QFile file(filePath);

    // Попытка открыть файл
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file.";
        return phasesMap;  // Возврат пустой карты, если файл не открыт
    }

    QXmlStreamReader xmlReader(&file);

    // Чтение XML-файла
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        // Проверка начала элемента
        if (token == QXmlStreamReader::StartElement) {
            QString elementName = xmlReader.name().toString();

            // Обработка элемента "Circuit" (общее напряжение)
            if (elementName == "Circuit") {
                QString voltageString = xmlReader.attributes().value("voltage").toString();
                bool ok;
                voltage = voltageString.toDouble(&ok);

                // Проверка успешности преобразования строки в число
                if (!ok) {
                    qDebug() << "Error: circuit must contain Voltage.";
                    file.close();
                    return phasesMap;
                }

                // Проверка на отрицательное или нулевое напряжение
                if (voltage <= 0) {
                    qDebug() << "Error: Voltage must be a positive value.";
                    file.close();
                    return phasesMap;
                }

                // Получение типа схемы
                circuitType = xmlReader.attributes().value("type").toString();

                // Проверка типа схемы
                if (circuitType.toLower() != "star" && circuitType.toLower() != "triangle") {
                    qDebug() << "Error: Unknown circuit type: " << circuitType;
                    file.close();
                    return phasesMap;
                }
            }

            // Обработка элемента "Phase"
            if (elementName == "Phase") {
                QString phaseName = xmlReader.attributes().value("name").toString();

                // Создание новой фазы, если ее еще нет в карте
                if (!phasesMap.contains(phaseName)) {
                    phasesMap[phaseName] = Phase();
                }

                Phase &phase = phasesMap[phaseName];

                // Флаг для отслеживания наличия компонентов в фазе
                bool phaseHasComponents = false;

                // Флаг для отслеживания наличия хотя бы одного ненулевого компонента в фазе
                bool phaseHasNonZeroComponent = false;

                // Чтение компонентов фазы
                while (!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "Phase")) {
                    if (xmlReader.tokenType() == QXmlStreamReader::StartElement) {
                        QString elementType = xmlReader.name().toString();

                        // Обработка компонентов (Resistor, Inductor, Capacitor)
                        if (elementType == "Resistor" || elementType == "Inductor" || elementType == "Capacitor") {
                            phaseHasComponents = true;

                            Component component;

                            // Получение атрибутов компонента
                            QXmlStreamAttributes componentAttributes = xmlReader.attributes();
                            component.name = componentAttributes.value("name").toString();
                            QString resistanceString = componentAttributes.value("resistance").toString();
                            bool ok;
                            component.resistance = resistanceString.toDouble(&ok);

                            // Проверка успешности преобразования строки в число
                            if (!ok) {
                                qDebug() << "Error converting string to number: " << resistanceString;
                                xmlReader.readNext();  // Пропускаем компонент с ошибкой и читаем следующий элемент
                                continue;
                            }

                            // Проверка на отрицательное сопротивление
                            if (component.resistance < 0) {
                                qDebug() << "Skipping component " << component.name << " due to negative resistance.";
                                xmlReader.readNext();  // Пропускаем компонент с отрицательным сопротивлением и читаем следующий элемент
                                continue;
                            }

                            // Проверка наличия ненулевого сопротивления
                            if (component.resistance != 0) {
                                phaseHasNonZeroComponent = true;
                            }

                            // Добавление компонента в соответствующий список фазы
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

                // Проверка наличия компонентов в фазе и отсутствия ошибок
                if (!phaseHasComponents || !phaseHasNonZeroComponent) {
                    qDebug() << "Skipping phase " << phaseName << " due to absence of components or all components have zero resistance.";
                    phasesMap.remove(phaseName);  // Удаление фазы из карты
                }

            }
        }
    }

    // Проверка наличия ошибок при чтении XML
    if (xmlReader.hasError()) {
        qDebug() << "XML parsing error: " << xmlReader.errorString();
    }

    // Закрытие файла
    file.close();

    // Возврат карты фаз
    return phasesMap;
}



QMap<QString, long double> calculateCurrent(const QMap<QString, Phase> &phasesMap, long double voltage, const QString &circuitType) {
    QMap<QString, long double> currentMap;

    // Рассчет силы тока для каждой фазы
    foreach (const QString &phaseName, phasesMap.keys()) {
        const Phase &phase = phasesMap[phaseName];

        long double totalResistance = 0.0;
        long double totalInductiveReactance = 0.0;
        long double totalCapacitiveReactance = 0.0;


        // Рассчет полного сопротивления Резистора
        foreach (const Component &resistor, phase.resistors) {
            totalResistance += resistor.resistance;
        }

        // Рассчет полного сопротивления Катушек
        foreach (const Component &inductor, phase.inductors) {
            totalInductiveReactance += 2 * M_PI * inductor.resistance;
        }

        // Рассчет полного сопротивления Конденсаторов
        foreach (const Component &capacitor, phase.capacitors) {
            if(capacitor.resistance != 0){
                totalCapacitiveReactance += 1 / (2 * M_PI * capacitor.resistance);
            }
        }

        // Рассчет полного Импеданса фазы
        long double totalImpedance = sqrt(pow(totalResistance, 2) + pow((totalInductiveReactance - totalCapacitiveReactance), 2));
        long double phaseVoltage = 0.0;
        long double current = 0.0;

        // Расчет силы тока для "Звезды" и "Треугольника"
        if (circuitType.toLower() == "star") {// Звезда
            phaseVoltage = voltage / sqrt(3);
            current = phaseVoltage / totalImpedance;
        } else if (circuitType.toLower() == "triangle") {// Треугольник
            phaseVoltage = voltage;
            current = phaseVoltage / totalImpedance;
        }

        qDebug() << "Phase " << phaseName << ":" << "Current: " << QString::number(current, 'g', 4) << " A";

        // Запись значения тока для текущей фазы
        currentMap[phaseName] = current;
    }

    return currentMap;
}



void writeCurrentToFile(const QMap<QString, long double> &currentMap, const QString &outputFilePath) {
    QFile outputFile(outputFilePath);

    // Попытка открыть файл для записи
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file for writing.";
        return;
    }

    QTextStream stream(&outputFile);

    foreach (const QString &phaseName, currentMap.keys()) {
        long double current = currentMap[phaseName];

        // Запись значения тока в файл
        stream << phaseName << " : " << QString::number(current, 'g', 4) << " A\n";
    }

    // Закрытие файла после записи
    outputFile.close();
}
