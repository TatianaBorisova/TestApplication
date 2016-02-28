#include "testview.h"
#include "randomgenerator.h"
#include "global.h"
#include "assureview.h"
#include "custombutton.h"

#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QMessageBox>
#include <QBuffer>
#include <QFileInfo>
#include <QDebug>

namespace {
const int fieldHeight = 50;
const int minHeight = 15;
}

TestView::TestView(QWidget *parent) :
    TestBaseView(parent),
    m_header(new QLabel(this)),
    m_questionEntry(new QLabel(this)),
    m_questionImg(new QLabel(this)),
    m_mainBox(new QVBoxLayout(this)),
    m_nextBtn(new QPushButton(this)),
    m_radioBtnList(new QGridLayout(this)),
    m_anssureView(new AssureView(this))
{
    m_anssureView->setVisible(false);
    m_questionImg->setVisible(false);

    m_questionEntry->setWordWrap(true);
    m_questionEntry->setAlignment(Qt::AlignCenter);

    connect(m_nextBtn,     &QPushButton::clicked, this, &TestView::setAnsweredState);
    connect(m_anssureView, &AssureView::yesButton, this, &TestView::yesVariant);
    connect(m_anssureView, &AssureView::noButton, this, &TestView::noVariant);

    this->setStyleSheet("font-family: Arial; font-style: normal; font-size: 20pt;");

    m_header->setText("Утверждение:");
    m_nextBtn->setText("Ответить");

    m_header->setFixedHeight(fieldHeight);
    m_header->setAlignment(Qt::AlignHCenter);
    m_nextBtn->setFixedSize(300, fieldHeight);

    m_mainBox->addWidget(m_header);
    m_mainBox->addWidget(m_questionEntry);
    m_mainBox->addWidget(m_questionImg);
    m_mainBox->addLayout(m_radioBtnList);
    m_mainBox->addWidget(m_nextBtn);

    m_mainBox->setAlignment(m_nextBtn,       Qt::AlignHCenter);
    m_mainBox->setAlignment(m_questionImg,   Qt::AlignHCenter);

    m_mainBox->setMargin(fieldHeight);
    m_mainBox->setSpacing(fieldHeight/2);

    setLayout(m_mainBox);
}

void TestView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void TestView::resize()
{
    m_questionImg->setFixedSize(height()*0.3, height()*0.3);
    m_radioBtnList->setContentsMargins(width()*0.2, 10, 10, 10);

    if (m_questionImg->pixmap())
        m_questionImg->setPixmap(m_questionImg->pixmap()->scaled(m_questionImg->height(), m_questionImg->height()));
}

void TestView::setTestData(const TestQuestions &question)
{
    m_answersList.clear();
    m_trueAnswer.clear();

    m_answer.isCorrectAnswer = false;
    m_answer.assurance = -1;
    m_answer.statement.clear();

    //set question text
    m_questionEntry->setText(question.question);
    m_answer.statement = question.question;

    //set answers
    QStringList answers;
    QStringList wrangAnswers = question.answers.uncorrectAnswers.split(";");
    m_trueAnswer = question.answers.correctAnswer;
    answers << question.answers.correctAnswer;
    for (int i = 0; i < wrangAnswers.count(); i++) {
        answers << wrangAnswers.at(i);
    }

    //set picture of question
    if (!question.answers.imgName.isEmpty())
        setImg(question.answers.image);
    else
        m_questionImg->hide();

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
    foreach (CustomButton *cbtn, CustomButton::customBtns()) {
        if (cbtn) {
            QRadioButton *btn = cbtn->radioBtn();
            if (btn && btn->isChecked()) {
                isChecked = true;
                m_answer.isCorrectAnswer = (m_trueAnswer.toLower() == cbtn->text().toLower());
            }
        }
    }
    if (!isChecked) {
        QMessageBox::warning(0, "Error", "Пожалуйста, выберите вариант ответа.");
        return;
    } else {
        hideEntry();
        m_anssureView->show();
    }
}

void TestView::hideEntry()
{
    m_questionEntry->hide();
    m_questionImg->hide();
    m_header->hide();
    m_nextBtn->hide();
    foreach (CustomButton *cbtn, CustomButton::customBtns()) {
        if (cbtn)
            cbtn->hide();
    }
}

void TestView::showEntry()
{
    m_questionEntry->show();
    m_header->show();
    m_nextBtn->show();
    foreach (CustomButton *cbtn, CustomButton::customBtns()) {
        if (cbtn)
            cbtn->show();
    }
}

void TestView::setImg(QByteArray img)
{
    QPixmap pixmap;
    pixmap.loadFromData(img);

    m_questionImg->setAlignment(Qt::AlignHCenter);
    m_questionImg->setPixmap(pixmap.scaled(m_questionImg->height(), m_questionImg->height()));
    m_questionImg->setVisible(true);
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
    CustomButton::deleteAllBtn();
}

void TestView::createRadioBtnList(const QStringList &list)
{
    clearRadioBtnList();

    if (list.count() <= 0) {
        QMessageBox::warning(0, "Error", "Ошибка тестирования.\nНе найдено вариантов ответов.");
        return;
    }

    int leftValue = 0;
    int rightValue = 0;

    // 0    0
    // 0    1
    // 1    0
    // 1    1
    for (int i = 0; i < list.count(); i++) {

        CustomButton *btn = new CustomButton(this);

        btn->setText(addUpperSymbol(list.at(i)));
        btn->setButtonId(i);

        CustomButton::addNewBtn(i, btn);

        if (i % 2) {
            m_radioBtnList->addWidget(btn, leftValue, 1);
            leftValue++;
        } else {
            m_radioBtnList->addWidget(btn, rightValue, 0);
            rightValue++;
        }

       // m_createdBtn.append(btn);
    }
}

QString TestView::addUpperSymbol(const QString &str)
{
    QString firstSymbol("");
    QString result = str;

    if (!result.isEmpty()) {
        firstSymbol = result.at(0).toUpper();
        result = result.remove(0, 1);
        result = firstSymbol + result;
    }
    return result;
}
