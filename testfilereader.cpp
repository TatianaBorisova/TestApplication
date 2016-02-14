#include "testfilereader.h"
#include "stringencryption.h"

#include <QList>
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

TestFileReader::TestFileReader(QObject *parent) :
    QObject(parent)
{

}

void TestFileReader::readTestFile(const QString &fileName)
{
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(0, "Error", "Can't open file: " + fileName);
            return;
        }

        const QString fileEntry = file.readAll();
        file.close();

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(fileEntry.toUtf8(), &error);

        if (doc.isEmpty()) {
            QMessageBox::warning(0, "Error", "Не могу прочесть данные из файла:\n" + fileName);
            return;
        }

        QJsonObject jsonObject = doc.object();

        QJsonArray jsonArray = jsonObject.value("testcontentarray").toArray();
        QList<TestStructure> testData;

        foreach (const QJsonValue &value, jsonArray) {
            QJsonObject obj = value.toObject();
            TestStructure testLines;

            testLines.question = StringEncryption::stringDecrypt(obj["TestQuestion"].toString(), "test");
            testLines.trueAnswer = StringEncryption::stringDecrypt(obj["CorrectTestAnswer"].toString(), "test");
            testLines.falseAnswer = StringEncryption::stringDecrypt(obj["TestAnswers"].toString(), "test");

            testData << testLines;
        }

        if (testData.count() > 0)
            emit readInfo(testData);
        else
            QMessageBox::warning(0, "Error", QString("Блоки данных в файле %1 пусты.\n").arg(fileName));
    }
}
