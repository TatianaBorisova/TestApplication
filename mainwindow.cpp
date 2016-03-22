#include "views/startview.h"
#include "views/maintesttabview.h"
#include "views/studentinfoview.h"
#include "views/testview.h"
#include "views/resultview.h"
#include "views/settingstabview.h"

#include "testfilereader.h"
#include "randomgenerator.h"
#include "tcpclient.h"
#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QObjectList>
#include <QDebug>
#include <QThread>
#include <QMessageBox>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    m_startWnd(new StartView(this)),
    m_chooseTest(new MainTestTabView(this)),
    m_studentData(new StudentInfoView(this)),
    m_fileReader(new TestFileReader(this)),
    m_testView(new TestView(this)),
    m_resultView(new ResultView(this)),
    m_settingsView(new SettingsTabView(this)),
    m_client(new TcpClient())
{
    hidePreviuosWindows();
    setFixedSize(getScreenGeometry().width()*0.9, getScreenGeometry().height()*0.9);
    move(0, 0);

    m_testView->setFixedSize(width(), height());
    this->setStyleSheet("font-family: Arial; font-style: normal; font-size: 15pt;");

    m_settingsView->setClientConnectionState(m_client->getErrorState());
    m_settingsView->setIp(m_client->getServerIp());
    m_settingsView->setPort(m_client->getServerPort());

    QIcon icon(":res/test.png");
    setWindowIcon(icon);

    //view connects
    connect(m_chooseTest, &MainTestTabView::chosenTestDB,   m_fileReader, &TestFileReader::readAllTestsFromDb);
    connect(m_fileReader, &TestFileReader::dbError,      m_chooseTest, &MainTestTabView::dbError);
    connect(m_chooseTest, &MainTestTabView::chosenTestName, m_fileReader, &TestFileReader::readTestFromDb);
    connect(m_fileReader, &TestFileReader::readTests,    m_chooseTest, &MainTestTabView::readTests);
    connect(m_fileReader, &TestFileReader::sendFullTestData, this,     &MainWindow::saveTestQuestions);

    connect(m_startWnd,   &StartView::showView,     this, &MainWindow::showTestView);
    connect(this,         &MainWindow::showView,    this, &MainWindow::showTestView);
    connect(m_chooseTest, &MainTestTabView::showView,  this, &MainWindow::showTestView);
    connect(m_settingsView, &SettingsTabView::showView, this, &MainWindow::showTestView);
    connect(m_resultView, &ResultView::showView,    this, &MainWindow::showTestView);

    connect(m_testView, &TestView::answeredResult,     this, &MainWindow::addAnswerToStudentInfoVector);
    connect(m_studentData, &StudentInfoView::nextStep, this, &MainWindow::updateStudentData);
    connect(this, &MainWindow::finishTestResult,            m_resultView, &ResultView::finishTestResult);

    creareClientThread();
}

void MainWindow::showTestView(TestAppView view)
{
    hidePreviuosWindows();
    setMainWindowSize(view);

    switch(view) {
    case TestStartView:
        m_startWnd->show();
        break;
    case TestWayView:
        m_chooseTest->show();
        break;
    case TestStudentInfoView:
        m_studentData->show();
        break;
    case TestEntryView:
        if (this->setTestData())
            m_testView->show();
        break;
    case TestResultView:
        m_resultView->show();
        break;
    case TestClientSettingsView:
        m_settingsView->show();
        break;
    default:
        break;
    }

    this->show();
}

void MainWindow::slotError(QAbstractSocket::SocketError err, const QString &errorStr)
{
    QString strError = "Error: " + (err == QAbstractSocket::HostNotFoundError
                                    ? "Сервер не найден."
                                    : err == QAbstractSocket::RemoteHostClosedError
                                      ? "Соединение с сервером закрыто."
                                      : err == QAbstractSocket::ConnectionRefusedError
                                        ? "Соединение прервано."
                                        : errorStr);


    QMessageBox::warning(0, "Error", strError);
}

void MainWindow::slotFileLoadingError(const QString &errMsg)
{
    QMessageBox::warning(0, "Error", errMsg);
}

void MainWindow::setMainWindowSize(TestAppView view)
{
    int mainW = 0;
    int mainH = 0;

    switch(view) {
    case TestStartView:
    case TestWayView:
    case TestStudentInfoView:
    case TestEntryView:
    case TestResultView:
    default:
        mainW = getScreenGeometry().width()*0.98;
        mainH = getScreenGeometry().height()*0.9;
        break;
    }
    setFixedSize(mainW, mainH);
}

bool MainWindow::setTestData()
{
    if (m_testList.questions.count() <= 0) {
        calculateRresult();
        emit finishTestResult(m_studentResult);
        emit showView(TestResultView);
        return false;
    }

    if (m_testList.questions.count() > 1) {
        int testNumber  = RandomGenerator::getValueInInterval(m_testList.questions.count());
        m_testView->setTestData(m_testList.questions.takeAt(testNumber));
    } else if (m_testList.questions.count() == 1) {
        m_testView->setTestData(m_testList.questions.takeLast());
    }
    return true;
}

//private
QRect MainWindow::getScreenGeometry() const
{
    return QApplication::desktop()->screenGeometry();
}

void MainWindow::hidePreviuosWindows()
{
    QObjectList children = this->children();
    for (int i = 0; i < children.count(); i++) {
        QWidget *child = dynamic_cast<QWidget*>(children.at(i));
        if (child) {
            child->hide();
        }
    }
}

void MainWindow::creareClientThread()
{
    qRegisterMetaType<StudentResult>("StudentResult" );
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

    QThread *thread = new QThread();
    m_client->moveToThread(thread);

    connect(this, &MainWindow::finishTestResult,            m_client,     &TcpClient::sendTestResultToServer, Qt::QueuedConnection);
    connect(m_settingsView, &SettingsTabView::startConnection,  m_client,     &TcpClient::tryConnectToHost, Qt::QueuedConnection);
    connect(m_settingsView, &SettingsTabView::refuseConnection, m_client,     &TcpClient::disconnectHost, Qt::QueuedConnection);
    connect(m_client, &TcpClient::connected, m_settingsView, &SettingsTabView::setClientConnectionState, Qt::QueuedConnection);
    connect(m_client, &TcpClient::connected, m_chooseTest,   &MainTestTabView::clientConnectionState, Qt::QueuedConnection);
    connect(m_client, &TcpClient::serverIpChanged,   m_settingsView, &SettingsTabView::setIp, Qt::QueuedConnection);
    connect(m_client, &TcpClient::serverPortChanged, m_settingsView, &SettingsTabView::setPort, Qt::QueuedConnection);
    connect(m_client, &TcpClient::fileLoadingError, this,   &MainWindow::slotFileLoadingError, Qt::QueuedConnection);
    connect(m_client, &TcpClient::error,            this,   &MainWindow::slotError);
    connect(m_chooseTest, &MainTestTabView::tryGetTestsFromServer, m_client,     &TcpClient::sendFilesGettingRequest, Qt::QueuedConnection);
    connect(m_client,     &TcpClient::sendDownloadedFilePath,   m_chooseTest, &MainTestTabView::sendDownloadedFilePath, Qt::QueuedConnection);

    connect(qApp, SIGNAL(aboutToQuit()), thread, SLOT(quit()));
    connect(thread,   &QThread::finished, m_client, &QThread::deleteLater);
    connect(thread,   &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}

void MainWindow::calculateRresult()
{
    qDebug() << m_studentResult.firstName
             << m_studentResult.secondName
             << m_studentResult.surname
             << m_studentResult.group;

    int score = 0;
    //calculate real score
    for (int i = 0; i < m_studentResult.answerInfo.count(); i++) {
        if (m_studentResult.answerInfo.at(i).isCorrectAnswer) {
            score += m_questionsWeight.at(i);
            if (m_studentResult.answerInfo.at(i).assurance)
                score++;
        }
    }

    m_studentResult.testName = m_testList.testName;
    m_studentResult.score = score;

    m_studentResult.maxPosibleScore = 0;
    //calculate MAX possible score
    for (int i = 0; i < m_studentResult.answerInfo.count(); i++) {
        m_studentResult.maxPosibleScore += m_questionsWeight.at(i);
        m_studentResult.maxPosibleScore++;
    }
    qDebug() << "m_studentResult.testName = " << m_studentResult.testName;
    qDebug() << "score" << m_studentResult.score << "max possible score " << m_studentResult.maxPosibleScore;
}

void MainWindow::saveTestQuestions(const TestData &testInfo)
{
    m_testList.id = -1;
    m_testList.questions.clear();
    m_testList.questionCount = -1;
    m_testList.testName.clear();
    m_testList.testTime.setHMS(0, 0, 0);
    m_testList.questions.clear();

    m_testList = testInfo;
    m_questionsWeight.clear();

    for (int i = 0; i < m_testList.questions.count(); i++) {
        m_questionsWeight.append(m_testList.questions.at(i).weight);
    }

    emit showView(TestStudentInfoView);
}

void MainWindow::updateStudentData(const StudentResult &data)
{
    m_studentData->hide();
    m_studentResult.answerInfo.clear();
    m_studentResult = data;

    emit showView(TestEntryView);
}

void MainWindow::addAnswerToStudentInfoVector(const AnswersVector &answer)
{
    m_studentResult.answerInfo.append(answer);
    emit showView(TestEntryView);
}


void MainWindow::startServerSearch()
{
    emit m_settingsView->startConnection(zeroHost, m_client->getServerPort());
}
