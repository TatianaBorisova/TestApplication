#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "global.h"

class StartView;
class ChooseTestView;
class TestFileReader;
class StudentInfoView;
class TestView;
class ResultView;
class TcpClient;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

public slots:
    void showTestChoice();
    void showStartScreen();
    void showStudentScreen();
    void showTestScreen();
    void showResultScreen();

signals:
    void startTestView();
    void finishTestResult(const StudentResult &result);

private slots:
    void saveTestQuestions(const QList<TestStructure> &testInfo);
    void updateStudentData(const StudentResult &data);
    void addAnswerToStudentInfoVector(const AnswersVector &answer);

private:
    QRect getScreenGeometry() const;
    void hidePreviuosWindows();
    void calculateRresult();

private:
    StartView           *m_startWnd;
    ChooseTestView      *m_chooseTest;
    StudentInfoView     *m_studentData;
    TestFileReader      *m_fileReader;
    QList<TestStructure> m_testList;
    StudentResult        m_studentResult;
    TestView            *m_testView;
    ResultView          *m_resultView;
    TcpClient           *m_client;
};

#endif // MAINWINDOW_H
