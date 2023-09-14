#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QCoreApplication>

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

#endif // FUNCTIONS_H
