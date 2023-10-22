#include "functions.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    if (argc < 3) {
        qDebug() << "The path to the XML file was not specified.";
        return 1;
    }

    QString filePath = QString::fromUtf8(argv[1]);

    long double totalVoltage = 0.0;

    QString circuitType;

    QMap<QString, Phase> result = parseXml(filePath, totalVoltage, circuitType);

    QString outputFilePath = QString::fromUtf8(argv[2]);

    if (!result.isEmpty()) {
        QMap<QString, long double> currentMap = calculateCurrent(result, totalVoltage, circuitType);

        writeCurrentToFile(currentMap, outputFilePath);
    }

    return a.exec();
}
