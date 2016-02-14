#include "assureview.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

namespace {
const int widthView = 400;
const int heightView = 300;
const int margin = 10;
const int btnWidth = 100;
const int btnHeight = 50;
}

AssureView::AssureView(QWidget *parent) :
    QWidget(parent),
    m_assure(new QLabel(this)),
    m_hbox(new QHBoxLayout(this)),
    m_yesBtn(new QPushButton(this)),
    m_noBtn(new QPushButton(this))
{
    setFixedSize(widthView, heightView);

    connect(m_yesBtn, &QPushButton::clicked, this, &AssureView::yesButton);
    connect(m_noBtn, &QPushButton::clicked, this, &AssureView::noButton);

    connect(m_yesBtn, &QPushButton::clicked, this, &AssureView::hide);
    connect(m_noBtn, &QPushButton::clicked, this, &AssureView::hide);

    QFont font("Times", 14);

    m_assure->setText("Вы уверенны в своем ответе?");
    m_assure->setFixedSize(widthView, heightView/5);
    m_assure->setFont(font);
    m_assure->setAlignment(Qt::AlignHCenter);

    m_assure->move(margin, margin*2);

    m_yesBtn->setText("Да");
    m_noBtn->setText("Нет");

    m_yesBtn->setFont(font);
    m_noBtn->setFont(font);

    m_yesBtn->setFixedSize(btnWidth, btnHeight);
    m_noBtn->setFixedSize(btnWidth, btnHeight);

    m_hbox->addWidget(m_yesBtn);
    m_hbox->addWidget(m_noBtn);

    m_hbox->setGeometry(QRect(0, m_assure->height() + margin*2, width(), height() - m_assure->height()));

    setLayout(m_hbox);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);;
}

