#include "functions.h"


QMap<QString, Phase> parseXml(const QString &filePath, long double &voltage, QString &circuitType) {
    QMap<QString, Phase> phasesMap;

    QFile file(filePath);

    QXmlStreamReader xmlReader(&file);

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
}
