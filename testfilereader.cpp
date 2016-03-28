#include "testfilereader.h"
#include "stringencryption.h"
#include "global.h"

#include <QList>
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <QDebug>

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

            testLines.question    = StringEncryption::stringDecrypt(obj["TestQuestion"].toString(), encryptKey);
            testLines.trueAnswer  = StringEncryption::stringDecrypt(obj["CorrectTestAnswer"].toString(), encryptKey);
            testLines.falseAnswer = StringEncryption::stringDecrypt(obj["TestAnswers"].toString(), encryptKey);
            testLines.imgPath     = StringEncryption::stringDecrypt(obj["ImgPath"].toString(), encryptKey);

            testData << testLines;
        }

        if (testData.count() > 0)
            emit readInfo(testData);
        else
            QMessageBox::warning(0, "Error", QString("Блоки данных в файле %1 пусты.\n").arg(fileName));
    }
}

void TestFileReader::readAllTestsFromDb(const QString &fileName)
{
    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");
    dbPtr.setDatabaseName(fileName);
    if (!dbPtr.open()) {
        QMessageBox::critical(0, "Can not open database", "Не могу открыть базу данных.\n");
        emit dbError();
        return;
    }

    m_Db = fileName;
    QList<TestHeaderData> list;
    //firstly lets check if test name already exists and delete it and all data connected with it
    QSqlQuery q_existed(dbPtr);
    q_existed.prepare("SELECT * FROM testdata");

    if (q_existed.exec()) {

        while (q_existed.next()) {

            TestHeaderData testData;
            testData.id            = q_existed.value(q_existed.record().indexOf("id")).toInt();
            testData.testName      = q_existed.value(q_existed.record().indexOf("testname")).toString();
            testData.testType      = (TestType)q_existed.value(q_existed.record().indexOf("testtype")).toInt();
            testData.testTime      = q_existed.value(q_existed.record().indexOf("testtime")).toTime();
            testData.questionCount = q_existed.value(q_existed.record().indexOf("questioncount")).toInt();
            list.append(testData);
        }
    }

    emit readTests(list);
    dbPtr.close();
}

void TestFileReader::readTestFromDb(const QString &testName)
{
    QSqlDatabase dbPtr = QSqlDatabase::addDatabase("QSQLITE");
    dbPtr.setDatabaseName(m_Db);
    if (!dbPtr.open()) {
        QMessageBox::critical(0, "Can not open database", "Не могу открыть базу данных.\n");
        emit dbError();
        return;
    }

    TestData data;
    QSqlQuery q_existed(dbPtr);

    q_existed.prepare("SELECT * FROM testdata WHERE testname=:testname");
    q_existed.bindValue(":testname", testName);

    if (q_existed.exec()) {
        while (q_existed.next()) {
            data.id            = q_existed.value(q_existed.record().indexOf("id")).toInt();
            data.testName      = q_existed.value(q_existed.record().indexOf("testname")).toString();
            data.testType      = (TestType)q_existed.value(q_existed.record().indexOf("testtype")).toInt();
            data.testTime      = q_existed.value(q_existed.record().indexOf("testtime")).toTime();
            data.questionCount = q_existed.value(q_existed.record().indexOf("questioncount")).toInt();
        }
    }

    QSqlQuery q_questions(dbPtr);
    q_questions.prepare("SELECT * FROM questionsdata WHERE testid=:testid");
    q_questions.bindValue(":testid", data.id);
    if (q_questions.exec()) {
        while (q_questions.next()) {
            TestQuestions question;
            question.question = StringEncryption::stringDecrypt(q_questions.value(q_questions.record().indexOf("question")).toString(), encryptKey);
            question.weight = q_questions.value(q_questions.record().indexOf("testweight")).toInt();
            question.answers.correctAnswer = StringEncryption::stringDecrypt(q_questions.value(q_questions.record().indexOf("correctanswer")).toString(), encryptKey);
            question.answers.uncorrectAnswers = StringEncryption::stringDecrypt(q_questions.value(q_questions.record().indexOf("uncorrectanswers")).toString(), encryptKey);
            question.answers.imgName = q_questions.value(q_questions.record().indexOf("imgname")).toString();
            question.answers.image = q_questions.value(q_questions.record().indexOf("image")).toByteArray();
            data.questions.append(question);
        }
    }

    emit sendFullTestData(data);
    dbPtr.close();
}
