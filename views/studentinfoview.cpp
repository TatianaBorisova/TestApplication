#include "studentinfoview.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

namespace {
const int margin = 5;
const int fieldHeight = 50;
}

StudentInfoView::StudentInfoView(QWidget *parent) :
    QWidget(parent),
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

    setFixedSize(middleWidth, middleHeight);

    m_centerBox->setContentsMargins(10*margin, margin, 45*margin, margin);
    m_centerBox->setGeometry(QRect(0, 0, width(), height()));

    m_nameLabel->setFixedSize((width()*0.2) - 2*margin, fieldHeight);
    m_secondNameLabel->setFixedSize((width()*0.2) - 2*margin, fieldHeight);
    m_surnameLabel->setFixedSize((width()*0.2) - 2*margin, fieldHeight);
    m_groupLabel->setFixedSize((width()*0.2) - 2*margin, fieldHeight);

    QFont font("Times", 16);

    m_nameLabel->setText("Имя:");
    m_secondNameLabel->setText("Отчество:");
    m_surnameLabel->setText("Фамилия:");
    m_groupLabel->setText("Группа:");

    m_nameLabel->setFont(font);
    m_secondNameLabel->setFont(font);
    m_surnameLabel->setFont(font);
    m_groupLabel->setFont(font);

    m_nameField->setFixedSize((width()/2) - 2*margin, fieldHeight);
    m_secondField->setFixedSize((width()/2) - 2*margin, fieldHeight);
    m_surnameField->setFixedSize((width()/2) - 2*margin, fieldHeight);
    m_groupField->setFixedSize((width()/2) - 2*margin, fieldHeight);

    m_nameField->setFont(font);
    m_secondField->setFont(font);
    m_surnameField->setFont(font);
    m_groupField->setFont(font);

    m_info->setFixedSize(width() - 2*margin, 2*fieldHeight);
    m_nextButton->setFixedSize(100, fieldHeight);

    m_info->setText("Пожалуйста, заполните данные для тестирования.");
    m_info->setFont(font);

    m_nextButton->setText("Далее");
    m_nextButton->setFont(font);

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

    this->setLayout(m_centerBox);
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
