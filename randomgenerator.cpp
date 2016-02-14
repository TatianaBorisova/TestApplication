#include "randomgenerator.h"

#include <QTime>
#include <qmath.h>

RandomGenerator::RandomGenerator(QObject *parent) :
    QObject(parent)
{
}

int RandomGenerator::getValueInInterval(int interval)
{
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    return qrand()%interval;
}
