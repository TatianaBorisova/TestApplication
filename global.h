#ifndef GLOBAL
#define GLOBAL

#include <QString>
#include <QList>
#include <QTime>

const int middleWidth = 800;
const int middleHeight = 600;
const int smallWidth = 300;
const int smallHeight = 300;

const QString cmdMsg = QString("GETFILES");
const QString downloadMsg = QString("DOWNLOADFILE");
const QString newfileMsg = QString("NEWFILENAME");
const QString newentryMsg = QString("NEWFILEENTRY");
const int headerMsgSize = 16;

//encryption
const QString encryptKey = QString("test");
const QString zeroHost = QString("0.0.0.0");

enum TestType {
    NoTypeTest = -1,
    StatementTest = 0,
    QuestionTest
};

typedef struct  test{
    QString question;
    QString trueAnswer;
    QString falseAnswer;
    QString imgPath;
} TestStructure ;

typedef struct answers {
    int     id;
    QString statement;
    QString chosenAnswer;
    int     isCorrectAnswer;
    int     assurance;
} AnswersVector;

typedef struct student {
    int id;
    QString testName;
    QString firstName;
    QString secondName;
    QString surname;
    QString group;
    int score;
    int maxPosibleScore;
    QString time;
    QList <AnswersVector> answerInfo;
} StudentResult;

typedef struct testAnswers {
    QString    correctAnswer;
    QString    uncorrectAnswers;
    QString    imgName;
    QByteArray image;
} Answers;

typedef struct questions {
    QString question;
    int     weight;
    Answers answers;
} TestQuestions;

typedef struct testSt {
    int      id;
    QString  testName;
    TestType testType;
    QTime    testTime;
    int      questionCount;
    QList<TestQuestions> questions;
} TestData;

typedef struct testHeader {
    int      id;
    QString  testName;
    TestType testType;
    QTime    testTime;
    int      questionCount;
} TestHeaderData;

enum TestAppView{
    TestStartView = 0,
    TestWayView,
    TestStudentInfoView,
    TestStatementEntryView,
    TestQuestionEntryView,
    TestResultView,
    TestClientSettingsView
};

#endif // GLOBAL

