#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "global.h"

class StartView;
class SettingsView;
class TestFileReader;
class StudentInfoView;
class TestView;
class ResultView;
class ClientTabView;
class TcpClient;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

public slots:
    void showTestView(TestAppView view);

signals:
    void showView(TestAppView view);
    void startTestView();
    void finishTestResult(const StudentResult &result);

private slots:
    void saveTestQuestions(const TestData &testInfo);
    void updateStudentData(const StudentResult &data);
    void addAnswerToStudentInfoVector(const AnswersVector &answer);

private:
    void setMainWindowSize(TestAppView view);
    bool setTestData();
    QRect getScreenGeometry() const;
    void hidePreviuosWindows();
    void calculateRresult();

private:
    StartView           *m_startWnd;
    SettingsView        *m_chooseTest;
    StudentInfoView     *m_studentData;
    TestFileReader      *m_fileReader;
    TestData             m_testList;
    StudentResult        m_studentResult;
    TestView            *m_testView;
    ResultView          *m_resultView;
    ClientTabView       *m_clientView;
    TcpClient           *m_client;
};

#endif // MAINWINDOW_H
