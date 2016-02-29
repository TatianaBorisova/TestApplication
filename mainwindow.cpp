#include "views/startview.h"
#include "views/settingsview.h"
#include "views/studentinfoview.h"
#include "views/testview.h"
#include "views/resultview.h"

#include "testfilereader.h"
#include "randomgenerator.h"
#include "tcpclient.h"
#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QObjectList>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    m_startWnd(new StartView(this)),
    m_chooseTest(new SettingsView(this)),
    m_studentData(new StudentInfoView(this)),
    m_fileReader(new TestFileReader(this)),
    m_testView(new TestView(this)),
    m_resultView(new ResultView(this)),
    m_client(new TcpClient(this))
{
    hidePreviuosWindows();
    setFixedSize(getScreenGeometry().width()*0.9, getScreenGeometry().height()*0.9);
    move(0, 0);

    m_testView->setFixedSize(width(), height());
    this->setStyleSheet("font-family: Arial; font-style: normal; font-size: 15pt;");

    //view connects
    connect(m_chooseTest, &SettingsView::chosenTestDB,   m_fileReader, &TestFileReader::readAllTestsFromDb);
    connect(m_fileReader, &TestFileReader::dbError,      m_chooseTest, &SettingsView::dbError);
    connect(m_chooseTest, &SettingsView::chosenTestName, m_fileReader, &TestFileReader::readTestFromDb);
    connect(m_fileReader, &TestFileReader::readTests,    m_chooseTest, &SettingsView::readTests);
    connect(m_fileReader, &TestFileReader::sendFullTestData, this,         &MainWindow::saveTestQuestions);
    connect(m_startWnd,   &StartView::showView,    this, &MainWindow::showTestView);
    connect(this,         &MainWindow::showView,   this, &MainWindow::showTestView);
    connect(m_chooseTest, &SettingsView::showView, this, &MainWindow::showTestView);

    connect(m_testView, &TestView::answeredResult, this, &MainWindow::addAnswerToStudentInfoVector);
    connect(m_studentData, &StudentInfoView::nextStep, this, &MainWindow::updateStudentData);

    connect(this, &MainWindow::finishTestResult, m_resultView, &ResultView::finishTestResult);
    connect(this, &MainWindow::finishTestResult, m_client, &TcpClient::sendToServer);
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
    default:
        break;
    }

    this->show();
}

void MainWindow::setMainWindowSize(TestAppView view)
{
    int mainW = 0;
    int mainH = 0;

    switch(view) {
    case TestStartView:
    case TestWayView:
        mainW = getScreenGeometry().width()*0.5;
        mainH = getScreenGeometry().height()*0.5;
        break;
    case TestStudentInfoView:
    case TestEntryView:
    case TestResultView:
    default:
        mainW = getScreenGeometry().width()*0.9;
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
    m_studentResult.score = score;

    qDebug() << "score" << m_studentResult.score;
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
