#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTcpSocket>

#include "views/testbaseview.h"
#include "global.h"

class StartView;
class MainTestView;
class TestFileReader;
class StudentInfoView;
class StatementTestView;
class ResultView;
class SettingsView;
class TcpClient;
class QuestionTestView;
class QSettings;

class MainWindow : public TestBaseView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    void startServerSearch();

public slots:
    void showTestView(TestAppView view);
    void slotError(QAbstractSocket::SocketError err, const QString &errorStr);

signals:
    void showView(TestAppView view);
    void startTestView();
    void finishTestResult(const StudentResult &result);

private slots:
    void saveTestQuestions(const TestData &testInfo);
    void updateStudentData(const StudentResult &data);
    void addAnswerToStudentInfoVector(const AnswersVector &answer);
    void slotFileLoadingError(const QString &errMsg);

private:
    void setMainWindowSize();
    bool setTestData();
    QRect getScreenGeometry() const;
    void hidePreviuosWindows();
    void calculateRresult();
    void creareClientThread();

protected:
    virtual void resize();

private:
    StartView         *m_startWnd;
    MainTestView      *m_chooseTest;
    StudentInfoView   *m_studentData;
    TestFileReader    *m_fileReader;
    TestData           m_testList;
    StudentResult      m_studentResult;
    StatementTestView *m_statementTestView;
    QuestionTestView  *m_questionTestView;
    ResultView        *m_resultView;
    SettingsView      *m_settingsView;
    TcpClient         *m_client;
    QList<int>         m_questionsWeight;
    QSettings         *m_screenSettings;
};

#endif // MAINWINDOW_H
