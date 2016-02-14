#include "startview.h"
#include "global.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QRect>
#include <QApplication>

StartView::StartView(QWidget *parent)
    : QWidget(parent),
      m_vbox(new QVBoxLayout(this)),
      m_openTest(new QPushButton("Выбрать тест", this)),
      m_exit(new QPushButton("Выход", this))
{   
    connect(m_exit, &QPushButton::clicked, this, &QApplication::quit);
    connect(m_openTest, &QPushButton::clicked, this, &StartView::openTestChoice);

    setFixedSize(smallWidth - 100, smallHeight);

    QFont font("Times", 14);

    m_openTest->setFont(font);
    m_exit->setFont(font);

    m_vbox->setGeometry(QRect(0, 0, this->width(), this->height()));

    m_openTest->setFixedSize(this->width() - m_vbox->margin(), 50);
    m_exit->setFixedSize(this->width() - m_vbox->margin(), 50);

    m_vbox->setMargin(5);
    m_vbox->addWidget(m_openTest);
    m_vbox->addWidget(m_exit);

    this->setLayout(m_vbox);
}

StartView::~StartView()
{
}
