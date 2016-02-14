#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <QObject>

class RandomGenerator : public QObject
{
    Q_OBJECT
public:
    explicit RandomGenerator(QObject *parent = 0);

    static int getValueInInterval(int interval);
};

#endif // RANDOMGENERATOR_H
