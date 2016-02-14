#include "views/startview.h"
#include "views/choosetestview.h"
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
    m_chooseTest(new ChooseTestView(this)),
    m_studentData(new StudentInfoView(this)),
    m_fileReader(new TestFileReader(this)),
    m_testView(new TestView(this)),
    m_resultView(new ResultView(this)),
    m_client(new TcpClient(this))
{
    hidePreviuosWindows();

    //view connects
    connect(m_startWnd, &StartView::openTestChoice, this, &MainWindow::showTestChoice);
    connect(m_chooseTest, &ChooseTestView::chosenTestName, m_fileReader, &TestFileReader::readTestFile);
    connect(this, &MainWindow::startTestView, this, &MainWindow::showTestScreen);
    connect(this, &MainWindow::finishTestResult, this, &MainWindow::showResultScreen);

    connect(this, &MainWindow::finishTestResult, m_resultView, &ResultView::finishTestResult);
    connect(m_testView, &TestView::answeredResult, this, &MainWindow::addAnswerToStudentInfoVector);
    connect(m_fileReader, &TestFileReader::readInfo, this, &MainWindow::saveTestQuestions);
    connect(m_fileReader, &TestFileReader::readInfo, this, &MainWindow::showStudentScreen);
    connect(m_studentData, &StudentInfoView::nextStep, this, &MainWindow::updateStudentData);
    connect(this, &MainWindow::finishTestResult, m_client, &TcpClient::sendToServer);
}

//public
void MainWindow::showStartScreen()
{
    hidePreviuosWindows();

    setFixedSize(smallWidth, smallHeight);

    this->move(getScreenGeometry().width()*0.2, getScreenGeometry().height()*0.2);
    m_startWnd->move(width()/2 - m_startWnd->width()/2, height()/2 - m_startWnd->height()/2);

    m_startWnd->show();
    this->show();
}

void MainWindow::showStudentScreen()
{
    hidePreviuosWindows();

    this->move(getScreenGeometry().width()*0.2, getScreenGeometry().height()*0.2);

    m_studentData->show();
    this->show();
}

void MainWindow::showResultScreen()
{
    hidePreviuosWindows();

    this->move(getScreenGeometry().width()*0.2, getScreenGeometry().height()*0.2);
    m_resultView->move((this->width() - m_resultView->width())/2, (this->height() - m_resultView->height())/2);

    m_resultView->show();
    this->show();
}

void MainWindow::showTestScreen()
{
    hidePreviuosWindows();

    this->move(getScreenGeometry().width()*0.2, getScreenGeometry().height()*0.2);

    if (m_testList.count() <= 0) {
        calculateRresult();
        emit finishTestResult(m_studentResult);
        return;
    }

    if (m_testList.count() > 1) {

        int testNumber = RandomGenerator::getValueInInterval(m_testList.count());
        m_testView->setTestData(m_testList.takeAt(testNumber));
        m_testView->show();

    } else if (m_testList.count() == 1) {

        m_testView->setTestData(m_testList.takeLast());
        m_testView->show();
    }

    this->show();
}

void MainWindow::showTestChoice()
{
    hidePreviuosWindows();

    setFixedSize(m_chooseTest->width(), m_chooseTest->height());

    this->move(getScreenGeometry().width()*0.2, getScreenGeometry().height()*0.2);

    m_chooseTest->show();
    this->show();
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

void MainWindow::saveTestQuestions(const QList<TestStructure> &testInfo)
{
    m_testList.clear();
    m_testList = testInfo;
}

void MainWindow::updateStudentData(const StudentResult &data)
{
    m_studentData->hide();
    m_studentResult.answerInfo.clear();
    m_studentResult = data;

    emit startTestView();
}

void MainWindow::addAnswerToStudentInfoVector(const AnswersVector &answer)
{
    m_studentResult.answerInfo.append(answer);
    emit startTestView();
}
