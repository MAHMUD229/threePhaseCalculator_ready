#include "functions.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QString filePath = QString::fromUtf8(argv[1]);
    QString outputFilePath = QString::fromUtf8(argv[2]);

    long double totalVoltage = 0.0;

    QString circuitType;

    QMap<QString, Phase> result = parseXml(filePath, totalVoltage, circuitType);

    if (!result.isEmpty()) {
        QMap<QString, long double> currentMap = calculateCurrent(result, totalVoltage, circuitType);

        writeCurrentToFile(currentMap, outputFilePath);
    }

    return a.exec();
}
