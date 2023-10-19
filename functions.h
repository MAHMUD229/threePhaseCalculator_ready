#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QCoreApplication>
#include <QXmlStreamReader>
#include <QMap>
#include <QFile>
#include <QDebug>
#include <cmath>

/*!
 * \struct Component
 * \brief Компонент электрической цепи (резистор, катушка, конденсатор)
 *
 * Компонент представляет собой элемент цепи с определенными характеристиками, такими как
 * имя, сопротивление (в омах) и тип (резистор, катушка, конденсатор).
 */
struct Component {
    QString name;
    long double resistance;
};

/*!
 * \struct Phase
 * \brief Фаза электрической цепи
 *
 * Фаза представляет собой группу компонентов, объединенных в рамках определенной фазы цепи.
 * Включает в себя резисторы, катушки и конденсаторы.
 */
struct Phase {
    QList<Component> resistors;
    QList<Component> inductors;
    QList<Component> capacitors;
};

/*!
 * \brief Парсинг XML-файла с описанием электрической цепи
 *
 * Функция выполняет парсинг XML-файла, содержащего описание электрической цепи.
 * Извлекает информацию о напряжении и компонентах цепи для каждой фазы.
 *
 * \param[in] filePath Путь к XML-файлу с описанием цепи.
 * \param[in, out] voltage Выходной параметр, в который будет записано напряжение цепи.
 * \param[in, out] circuitType - Выходной параметр, в который будет записан тип трехфазной цепи: "Треугольник" или "Звезда".
 * \return Карта фаз цепи, содержащая списки резисторов, катушек и конденсаторов для каждой фазы.
 *         В случае ошибки возвращает пустую карту фаз.
 */
QMap<QString, Phase> parseXml(const QString &filePath, long double &voltage, QString &circuitType);

/*!
 * \brief Расчет тока в электрической цепи
 *
 * Функция проводит расчет силы тока в электрической цепи переменного тока.
 * Принимает входные данные о компонентах цепи и ее напряжении.
 *
 * \param[in] phasesMap Карта фаз цепи, содержащая списки резисторов, катушек и конденсаторов для каждой фазы.
 * \param[in] voltage Напряжение цепи переменного тока.
 * \param[in] circuitType - Входной параметр, в котором записан тип трехфазной цепи: "Треугольник" или "Звезда".
 */
QMap<QString, long double> calculateCurrent(const QMap<QString, Phase> &phasesMap, long double voltage, const QString &circuitType);



#endif // FUNCTIONS_H
