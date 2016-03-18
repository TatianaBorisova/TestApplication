#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTcpSocket>

#include "global.h"

class StartView;
class SettingsView;
class TestFileReader;
class StudentInfoView;
class TestView;
class ResultView;
class ClientTabView;
class TcpClient;
class DownloadManager;

class MainWindow : public QWidget
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
    void slotFileLoadingError();

private:
    void setMainWindowSize(TestAppView view);
    bool setTestData();
    QRect getScreenGeometry() const;
    void hidePreviuosWindows();
    void calculateRresult();
    void creareClientThread();

private:
    StartView       *m_startWnd;
    SettingsView    *m_chooseTest;
    StudentInfoView *m_studentData;
    TestFileReader  *m_fileReader;
    TestData         m_testList;
    StudentResult    m_studentResult;
    TestView        *m_testView;
    ResultView      *m_resultView;
    ClientTabView   *m_clientView;
    TcpClient       *m_client;
};

#endif // MAINWINDOW_H
