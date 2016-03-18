#include "views/startview.h"
#include "views/settingsview.h"
#include "views/studentinfoview.h"
#include "views/testview.h"
#include "views/resultview.h"
#include "views/clienttabview.h"

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

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    m_startWnd(new StartView(this)),
    m_chooseTest(new SettingsView(this)),
    m_studentData(new StudentInfoView(this)),
    m_fileReader(new TestFileReader(this)),
    m_testView(new TestView(this)),
    m_resultView(new ResultView(this)),
    m_clientView(new ClientTabView(this)),
    m_client(new TcpClient())
{
    hidePreviuosWindows();
    setFixedSize(getScreenGeometry().width()*0.9, getScreenGeometry().height()*0.9);
    move(0, 0);

    m_testView->setFixedSize(width(), height());
    this->setStyleSheet("font-family: Arial; font-style: normal; font-size: 15pt;");

    m_clientView->setClientConnectionState(m_client->getErrorState());
    m_clientView->setIp(m_client->getServerIp());
    m_clientView->setPort(m_client->getServerPort());

    //view connects
    connect(m_chooseTest, &SettingsView::chosenTestDB,   m_fileReader, &TestFileReader::readAllTestsFromDb);
    connect(m_fileReader, &TestFileReader::dbError,      m_chooseTest, &SettingsView::dbError);
    connect(m_chooseTest, &SettingsView::chosenTestName, m_fileReader, &TestFileReader::readTestFromDb);
    connect(m_fileReader, &TestFileReader::readTests,    m_chooseTest, &SettingsView::readTests);
    connect(m_fileReader, &TestFileReader::sendFullTestData, this,     &MainWindow::saveTestQuestions);

    connect(m_startWnd,   &StartView::showView,     this, &MainWindow::showTestView);
    connect(this,         &MainWindow::showView,    this, &MainWindow::showTestView);
    connect(m_chooseTest, &SettingsView::showView,  this, &MainWindow::showTestView);
    connect(m_clientView, &ClientTabView::showView, this, &MainWindow::showTestView);
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
        m_clientView->show();
        break;
    default:
        break;
    }

    this->show();
}

void MainWindow::slotError(QAbstractSocket::SocketError err, const QString &errorStr)
{
    QString strError = "Error: " + (err == QAbstractSocket::HostNotFoundError
                                    ? "The host was not found."
                                    : err == QAbstractSocket::RemoteHostClosedError
                                      ? "The remote host is closed."
                                      : err == QAbstractSocket::ConnectionRefusedError
                                        ? "The connection was refused."
                                        : errorStr);


    QMessageBox::warning(0, "Error", strError);
}

void MainWindow::slotFileLoadingError()
{
    QMessageBox::warning(0, "Error", "Тестовые файлы не найдены на сервере.");
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
        int testNumber = RandomGenerator::getValueInInterval(m_testList.questions.count());
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
    connect(m_clientView, &ClientTabView::startConnection,  m_client,     &TcpClient::tryConnectToHost, Qt::QueuedConnection);
    connect(m_clientView, &ClientTabView::refuseConnection, m_client,     &TcpClient::disconnectHost, Qt::QueuedConnection);
    connect(m_client, &TcpClient::connected, m_clientView, &ClientTabView::setClientConnectionState, Qt::QueuedConnection);
    connect(m_client, &TcpClient::connected, m_chooseTest, &SettingsView::clientConnectionState, Qt::QueuedConnection);
    connect(m_client, &TcpClient::serverIpChanged,   m_clientView, &ClientTabView::setIp, Qt::QueuedConnection);
    connect(m_client, &TcpClient::serverPortChanged, m_clientView, &ClientTabView::setPort, Qt::QueuedConnection);
    connect(m_client, &TcpClient::fileLoadingError, this,   &MainWindow::slotFileLoadingError, Qt::QueuedConnection);
    connect(m_client, &TcpClient::error,    this,   &MainWindow::slotError);
    connect(m_chooseTest, &SettingsView::tryGetTestsFromServer, m_client, &TcpClient::sendFilesGettingRequest, Qt::QueuedConnection);

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
    //calculate score
    for (int i = 0; i < m_studentResult.answerInfo.count(); i++) {
        if (m_studentResult.answerInfo.at(i).isCorrectAnswer) {
            score++;
            if (m_studentResult.answerInfo.at(i).assurance)
                score++;
        }
    }

    m_studentResult.testName = m_testList.testName;
    m_studentResult.score = score;
    m_studentResult.maxPosibleScore = m_studentResult.answerInfo.count()*2; //double questions count

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
    emit m_clientView->startConnection(zeroHost, m_client->getServerPort());
}
