#ifndef GLOBAL
#define GLOBAL

#include <QString>
#include <QList>

const int middleWidth = 800;
const int middleHeight = 600;
const int smallWidth = 300;
const int smallHeight = 300;

const int headerMsgSize = 16;

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
    QString firstName;
    QString secondName;
    QString surname;
    QString group;
    int score;
    QList<AnswersVector> answerInfo;
} StudentResult;

enum TestAppView{
    TestStartView = 0,
    TestWayView,
    TestStudentInfoView,
    TestEntryView,
    TestResultView
};

#endif // GLOBAL

