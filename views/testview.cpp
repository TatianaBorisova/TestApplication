#include "testview.h"
#include "randomgenerator.h"
#include "global.h"
#include "assureview.h"

#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QMessageBox>

namespace {
const int fieldHeight = 50;
const int margin = 15;
}

TestView::TestView(QWidget *parent) :
    QWidget(parent),
    m_header(new QLabel(this)),
    m_questionEntry(new QLabel(this)),
    m_mainBox(new QVBoxLayout(this)),
    nextBtn(new QPushButton(this)),
    radioBtnList(new QGridLayout(this)),
    m_anssureView(new AssureView(this))
{
    m_anssureView->setVisible(false);

    setFixedSize(middleWidth, middleHeight);

    connect(nextBtn, &QPushButton::clicked, this, &TestView::setAnsweredState);
    connect(m_anssureView, &AssureView::yesButton, this, &TestView::yesVariant);
    connect(m_anssureView, &AssureView::noButton, this, &TestView::noVariant);

    m_mainBox->setMargin(margin);

    m_mainBox->setGeometry(QRect(0, 0, width(), height()));
    radioBtnList->setGeometry(QRect(0, height()/2, width(), height()/2));

    m_header->setFixedSize(width(), fieldHeight/2);
    m_questionEntry->setFixedSize(width() - 2*margin, height()*0.3);
    nextBtn->setFixedSize(100, fieldHeight);

    QFont font("Times", 16);

    m_header->setText("Утверждение:");
    nextBtn->setText("Ответить");

    m_header->setAlignment(Qt::AlignHCenter);
    m_questionEntry->setAlignment(Qt::AlignHCenter);

    m_header->setFont(font);
    m_questionEntry->setFont(font);
    nextBtn->setFont(font);

    m_mainBox->addWidget(m_header);
    m_mainBox->addWidget(m_questionEntry);
    m_mainBox->addLayout(radioBtnList);
    m_mainBox->addWidget(nextBtn);

    this->setLayout(m_mainBox);
}

void TestView::setTestData(const TestStructure &question)
{
    m_answersList.clear();
    m_trueAnswer.clear();

    m_answer.isCorrectAnswer = false;
    m_answer.assurance = -1;
    m_answer.statement.clear();

    QStringList answers;

    m_questionEntry->setText(question.question);
    m_answer.statement = question.question;

    QStringList wrangAnswers = question.falseAnswer.split(";");

    m_trueAnswer = question.trueAnswer;
    answers << question.trueAnswer;

    for (int i = 0; i < wrangAnswers.count(); i++) {
        answers << wrangAnswers.at(i);
    }

    while (answers.count() > 0) {
        if (answers.count() > 1) {
            int val = RandomGenerator::getValueInInterval(answers.count());
            m_answersList << answers.takeAt(val);
        } else {
            m_answersList << answers.takeLast();
        }
    }

    createRadioBtnList(m_answersList);
}

void TestView::setAnsweredState()
{
    bool isChecked = false;
    for (int i = 0; i < m_createdBtn.count(); i++) {
        QRadioButton *btn = m_createdBtn.at(i);
        if (btn && btn->isChecked()) {
            isChecked = true;
            m_answer.isCorrectAnswer = (m_trueAnswer == btn->text());
        }
    }

    if (!isChecked) {
        QMessageBox::warning(0, "Error", "Пожалуйста, выберите вариант ответа.");
        return;
    } else {
        hideEntry();
        m_anssureView->move((this->width() - m_anssureView->width())/2, (this->height() - m_anssureView->height())/2);
        m_anssureView->show();
    }
}

void TestView::hideEntry()
{
    m_questionEntry->hide();
    m_header->hide();
    nextBtn->hide();
    for (int i = 0; i < m_createdBtn.count(); i++) {
        m_createdBtn.at(i)->hide();
    }
}

void TestView::showEntry()
{
    m_questionEntry->show();
    m_header->show();
    nextBtn->show();
    for (int i = 0; i < m_createdBtn.count(); i++) {
        m_createdBtn.at(i)->show();
    }
}

void TestView::yesVariant()
{
    if (m_answer.isCorrectAnswer)
        m_answer.assurance = 1;
    else
        m_answer.assurance = 0;

    emit answeredResult(m_answer);
    showEntry();
}

void TestView::noVariant()
{
    m_answer.assurance = 0;
    emit answeredResult(m_answer);
    showEntry();
}

void TestView::clearRadioBtnList()
{
    for (int i = 0; i < m_createdBtn.count(); i++) {
        QRadioButton *btn = m_createdBtn.at(i);
        if (btn) {
            delete btn;
        }
    }
    m_createdBtn.clear();
}

void TestView::createRadioBtnList(const QStringList &list)
{
    clearRadioBtnList();

    if (list.count() <= 0) {
        QMessageBox::warning(0, "Error", "Ошибка тестирования.\nНе найдено вариантов ответов.");
        return;
    }

    int leftColumn = 0;
    int leftRow = 0;

    int rightRow = 0;
    int rightColumn = 1;

    for (int i = 0; i < list.count(); i++) {
        QRadioButton *btn = new QRadioButton();
        btn->setText(list.at(i));

        QFont font("Times", 14);
        btn->setFont(font);

        if (i % 2) {
            radioBtnList->addWidget(btn, leftRow, leftColumn);
            leftRow++;
        } else {
            radioBtnList->addWidget(btn, rightRow, rightColumn);
            rightColumn++;
        }
        m_createdBtn.append(btn);
    }
}
