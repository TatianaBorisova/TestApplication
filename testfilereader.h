#ifndef TESTFILEREADER_H
#define TESTFILEREADER_H

#include "global.h"

#include <QObject>

class TestFileReader : public QObject
{
    Q_OBJECT
public:
    explicit TestFileReader(QObject *parent = 0);

public slots:
    void readTestFile(const QString &fileName);

signals:
    void readInfo(const QList<TestStructure> &testInfo);
};

#endif // TESTFILEREADER_H
