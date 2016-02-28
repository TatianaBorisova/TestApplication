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
    void readAllTestsFromDb(const QString &fileName);
    void readTestFromDb(const QString &testName);

signals:
    void readInfo(const QList<TestStructure> &testInfo);
    void readTests(const QList<TestHeaderData> &test);
    void sendFullTestData(const TestData &data);

private:
    QString m_Db;
};

#endif // TESTFILEREADER_H
