#include "assureview.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

namespace {
const int minHeight = 10;
const int btnWidth = 100;
const int btnHeight = 50;
const int assureViewWidth = 400;
}

AssureView::AssureView(QWidget *parent) :
    TestBaseView(parent),
    m_assure(new QLabel(this)),
    m_hbox(new QHBoxLayout(this)),
    m_vbox(new QVBoxLayout(this)),
    m_yesBtn(new QPushButton(this)),
    m_noBtn(new QPushButton(this))
{
    connect(m_yesBtn, &QPushButton::clicked, this, &AssureView::yesButton);
    connect(m_noBtn, &QPushButton::clicked, this, &AssureView::noButton);

    connect(m_yesBtn, &QPushButton::clicked, this, &AssureView::hide);
    connect(m_noBtn, &QPushButton::clicked, this, &AssureView::hide);

    setFixedWidth(assureViewWidth);

    m_assure->setText("Вы уверены в своем ответе?");
    m_assure->setAlignment(Qt::AlignHCenter);
    m_assure->setFixedWidth(width());

    m_yesBtn->setText("Да");
    m_noBtn->setText("Нет");

    m_yesBtn->setFixedSize(btnWidth, btnHeight);
    m_noBtn->setFixedSize(btnWidth, btnHeight);

    m_hbox->addWidget(m_yesBtn);
    m_hbox->addWidget(m_noBtn);

    m_vbox->addWidget(m_assure);
    m_vbox->addLayout(m_hbox);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    setLayout(m_vbox);
}

void AssureView::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent) {
        setFixedHeight(wParent->height()*0.7);
        move((wParent->width() - width())/2, (wParent->height() - height())/2);
    }

    m_vbox->setAlignment(m_assure, Qt::AlignHCenter);
    m_hbox->setSpacing(m_assure->width() - 2*m_yesBtn->width());
}
