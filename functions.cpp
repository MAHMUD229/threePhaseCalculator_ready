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
        }
    }
}
