#include "studentinfoview.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

namespace {
const int minHeight = 5;
const int fieldHeight = 50;
}

StudentInfoView::StudentInfoView(QWidget *parent) :
    TestBaseView(parent),
    m_centerBox(new QGridLayout(this)),
    m_nameLabel(new QLabel(this)),
    m_secondNameLabel(new QLabel(this)),
    m_surnameLabel(new QLabel(this)),
    m_groupLabel(new QLabel(this)),
    m_nameField(new QLineEdit(this)),
    m_secondField(new QLineEdit(this)),
    m_surnameField(new QLineEdit(this)),
    m_groupField(new QLineEdit(this)),
    m_info(new QLabel(this)),
    m_nextButton(new QPushButton(this))
{
    connect(m_nextButton, &QPushButton::clicked, this, &StudentInfoView::nextButtonPressed);

    this->setStyleSheet("font-family: Arial; font-style: normal; font-size: 20pt;");

    m_nameLabel->setText("Имя:");
    m_secondNameLabel->setText("Отчество:");
    m_surnameLabel->setText("Фамилия:");
    m_groupLabel->setText("Группа:");

    m_info->setText("Пожалуйста, заполните данные для тестирования.");
    m_nextButton->setText("Далее");

    m_centerBox->addWidget(m_nameLabel, 0, 0);
    m_centerBox->addWidget(m_secondNameLabel, 1, 0);
    m_centerBox->addWidget(m_surnameLabel, 2, 0);
    m_centerBox->addWidget(m_groupLabel, 3, 0);
    m_centerBox->addWidget(m_info, 4, 0);
    m_centerBox->addWidget(m_nextButton, 5, 0);

    m_centerBox->addWidget(m_nameField, 0, 1);
    m_centerBox->addWidget(m_secondField, 1, 1);
    m_centerBox->addWidget(m_surnameField, 2, 1);
    m_centerBox->addWidget(m_groupField, 3, 1);

    setLayout(m_centerBox);
}

void StudentInfoView::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent)
        setFixedSize(wParent->width(), wParent->height());

    m_centerBox->setContentsMargins(10*minHeight, minHeight, 45*minHeight, minHeight);
    m_centerBox->setGeometry(QRect(0, 0, width(), height()));

    m_nameLabel->setFixedSize((width()*0.2) - 2*minHeight, fieldHeight);
    m_secondNameLabel->setFixedSize((width()*0.2) - 2*minHeight, fieldHeight);
    m_surnameLabel->setFixedSize((width()*0.2) - 2*minHeight, fieldHeight);
    m_groupLabel->setFixedSize((width()*0.2) - 2*minHeight, fieldHeight);

    m_nameField->setFixedSize((width()/2) - 2*minHeight, fieldHeight);
    m_secondField->setFixedSize((width()/2) - 2*minHeight, fieldHeight);
    m_surnameField->setFixedSize((width()/2) - 2*minHeight, fieldHeight);
    m_groupField->setFixedSize((width()/2) - 2*minHeight, fieldHeight);

    m_info->setFixedSize(width() - 2*minHeight, 2*fieldHeight);
    m_nextButton->setFixedSize(100, fieldHeight);
}

void StudentInfoView::nextButtonPressed()
{
    if (m_nameField->text().isEmpty() ||
            m_secondField->text().isEmpty() ||
            m_surnameField->text().isEmpty() ||
            m_groupField->text().isEmpty()) {
        QMessageBox::warning(0, "error", "Пожалуйста, заполните все поля.");
    } else {
        StudentResult info;
        info.firstName  = m_nameField->text();
        info.secondName = m_secondField->text();
        info.surname    = m_surnameField->text();
        info.group      = m_groupField->text();

        emit nextStep(info);
    }
}
