#include "functions.h"


QMap<QString, Phase> parseXml(const QString &filePath, long double &voltage, QString &circuitType) {
    QMap<QString, Phase> phasesMap;

    QFile file(filePath);

    QXmlStreamReader xmlReader(&file);
}
