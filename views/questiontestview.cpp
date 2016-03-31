#include "questiontestview.h"
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
const int customBtnSize = 100;
const int minHeight = 15;
const int viewWidth = 600;
}

QuestionTestView::QuestionTestView(QWidget *parent) :
    TestBaseView(parent),
    m_header(new QLabel(this)),
    m_questionEntry(new QLabel(this)),
    m_questionImg(new QLabel(this)),
    m_mainBox(new QVBoxLayout(this)),
    m_answerBtn(new QPushButton(this)),
    m_answersView(new QWidget(this)),
    m_radioBtnList(new QGridLayout(m_answersView))
{
    m_questionImg->setVisible(false);

    m_questionEntry->setWordWrap(true);
    m_questionEntry->setAlignment(Qt::AlignCenter);

    connect(m_answerBtn,   &QPushButton::clicked,  this, &QuestionTestView::setAnsweredState);

    this->setStyleSheet("font-family: Arial; font-style: normal; font-size: 20pt;");

    m_header->setText("Утверждение:");
    m_answerBtn->setText("Ответить");

    m_header->setFixedHeight(fieldHeight);
    m_header->setAlignment(Qt::AlignHCenter);
    m_questionEntry->setAlignment(Qt::AlignCenter);
    m_answerBtn->setFixedSize(300, fieldHeight);
    m_answersView->setFixedWidth(viewWidth);

    m_mainBox->addWidget(m_header);
    m_mainBox->addWidget(m_questionEntry);
    m_mainBox->addWidget(m_questionImg);
    m_mainBox->addWidget(m_answersView);
    m_mainBox->addWidget(m_answerBtn);

    m_mainBox->setAlignment(m_questionEntry, Qt::AlignHCenter);
    m_mainBox->setAlignment(m_answersView,   Qt::AlignRight);
    m_mainBox->setAlignment(m_answerBtn,     Qt::AlignHCenter);
    m_mainBox->setAlignment(m_questionImg,   Qt::AlignHCenter);

    m_mainBox->setMargin(fieldHeight);
    m_mainBox->setSpacing(fieldHeight);

    m_radioBtnList->setSpacing(customBtnSize);
    setLayout(m_mainBox);
}

void QuestionTestView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    resize();
}

void QuestionTestView::resize()
{
    m_questionImg->setFixedSize(height()*0.3, height()*0.3);
    m_questionEntry->setFixedWidth(width()*0.7);
    m_answersView->setFixedWidth(width()*0.7);

    if (m_questionImg->pixmap())
        m_questionImg->setPixmap(m_questionImg->pixmap()->scaled(m_questionImg->height(), m_questionImg->height()));
}

void QuestionTestView::setTestData(const TestQuestions &question)
{
    m_answersList.clear();
    m_trueAnswer.clear();

    m_answer.isCorrectAnswer = 0;
    m_answer.assurance = -1;
    m_answer.statement.clear();

    //set question text
    m_questionEntry->setText(question.question);
    m_answer.statement = question.question;

    //set answers on view
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

void QuestionTestView::setAnsweredState()
{
    bool isChecked = false;
    foreach (CustomButton *cbtn, CustomButton::customBtns()) {
        if (cbtn) {
            QRadioButton *btn = cbtn->radioBtn();
            if (btn && btn->isChecked()) {
                isChecked = true;
                if (m_trueAnswer.toLower() == cbtn->text().toLower())
                    m_answer.isCorrectAnswer = 1;
                else
                    m_answer.isCorrectAnswer = 0;
                //save chosen answer for statistic info
                m_answer.chosenAnswer = cbtn->text();
            }
        }
    }
    if (!isChecked) {
        QMessageBox::warning(0, "Error", "Пожалуйста, выберите вариант ответа.");
        return;
    } else {
        hideEntry();
        m_answer.assurance = -1;
        emit answeredResult(m_answer);
        showEntry();
    }
}

void QuestionTestView::hideEntry()
{
    m_questionEntry->hide();
    m_questionImg->hide();
    m_header->hide();
    m_answerBtn->hide();
    foreach (CustomButton *cbtn, CustomButton::customBtns()) {
        if (cbtn)
            cbtn->hide();
    }
}

void QuestionTestView::showEntry()
{
    m_questionEntry->show();
    m_header->show();
    m_answerBtn->show();
    foreach (CustomButton *cbtn, CustomButton::customBtns()) {
        if (cbtn)
            cbtn->show();
    }
}

void QuestionTestView::setImg(QByteArray img)
{
    QPixmap pixmap;
    pixmap.loadFromData(img);

    m_questionImg->setAlignment(Qt::AlignHCenter);
    m_questionImg->setPixmap(pixmap.scaled(m_questionImg->height(), m_questionImg->height()));
    m_questionImg->setVisible(true);
}

void QuestionTestView::clearRadioBtnList()
{
    CustomButton::deleteAllBtn();
}

void QuestionTestView::createRadioBtnList(const QStringList &list)
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

        btn->setText(list.at(i));
        btn->setButtonId(i);

        CustomButton::addNewBtn(i, btn);

        if (i % 2) {
            m_radioBtnList->addWidget(btn, leftValue, 1);
            leftValue++;
        } else {
            m_radioBtnList->addWidget(btn, rightValue, 0);
            rightValue++;
        }
    }
}
