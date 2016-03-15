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
const int cmdSize = 8;
const int headerMsgSize = 16;

const QString zeroHost = QString("0.0.0.0");

typedef struct  test{
    QString question;
    QString trueAnswer;
    QString falseAnswer;
    QString imgPath;
} TestStructure ;

typedef struct answers {
    QString statement;
    bool isCorrectAnswer;
    int assurance;
} AnswersVector;

typedef struct student {
    QString testName;
    QString firstName;
    QString secondName;
    QString surname;
    QString group;
    int score;
    int maxPosibleScore;
    QList<AnswersVector> answerInfo;
} StudentResult;

typedef struct testAnswers {
    QString correctAnswer;
    QString uncorrectAnswers;
    QString imgName;
    QByteArray image;
} Answers;

typedef struct questions {
    QString question;
    int     weight;
    Answers answers;
} TestQuestions;

typedef struct testSt {
    int id;
    QString testName;
    QTime testTime;
    int questionCount;
    QList<TestQuestions> questions;
} TestData;

typedef struct testHeader {
    int id;
    QString testName;
    QTime testTime;
    int questionCount;
} TestHeaderData;

enum TestAppView{
    TestStartView = 0,
    TestWayView,
    TestStudentInfoView,
    TestEntryView,
    TestResultView,
    TestClientSettingsView
};

#endif // GLOBAL

