#include "assureview.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

namespace {
const int margin = 10;
const int btnWidth = 100;
const int btnHeight = 50;
}

AssureView::AssureView(QWidget *parent) :
    TestBaseView(parent),
    m_assure(new QLabel(this)),
    m_hbox(new QHBoxLayout(this)),
    m_yesBtn(new QPushButton(this)),
    m_noBtn(new QPushButton(this))
{
    connect(m_yesBtn, &QPushButton::clicked, this, &AssureView::yesButton);
    connect(m_noBtn, &QPushButton::clicked, this, &AssureView::noButton);

    connect(m_yesBtn, &QPushButton::clicked, this, &AssureView::hide);
    connect(m_noBtn, &QPushButton::clicked, this, &AssureView::hide);

    QFont font("Times", 14);

    m_assure->setText("Вы уверенны в своем ответе?");
    m_assure->setFont(font);
    m_assure->setAlignment(Qt::AlignHCenter);

    m_yesBtn->setText("Да");
    m_noBtn->setText("Нет");

    m_yesBtn->setFont(font);
    m_noBtn->setFont(font);

    m_yesBtn->setFixedSize(btnWidth, btnHeight);
    m_noBtn->setFixedSize(btnWidth, btnHeight);

    m_hbox->addWidget(m_yesBtn);
    m_hbox->addWidget(m_noBtn);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);

    setLayout(m_hbox);
}

void AssureView::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent) {
        setFixedSize(wParent->width()/2, wParent->height()*0.7);
        move((wParent->width() - width())/2, (wParent->height() - height())/2);
    }

    m_assure->setFixedSize(width(), height()*0.3);

    m_assure->move(0, 50);
}
