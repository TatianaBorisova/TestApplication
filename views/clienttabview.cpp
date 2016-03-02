#include "clienttabview.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QDebug>

ClientTabView::ClientTabView(QWidget *parent) :
    TestBaseView(parent),
    m_host(new QLabel("Сервер: ", this)),
    m_port(new QLabel("Порт: ", this)),
    m_hostBox(new QLineEdit(this)),
    m_portBox(new QLineEdit(this)),
    m_save(new QPushButton("Сохранить и подключиться", this)),
    m_back(new QPushButton("Назад", this)),
    m_connectionState(new QLabel("Статус: ", this)),
    m_box(new QGridLayout(this))
{
    connect(m_save, &QPushButton::clicked, this, &ClientTabView::saveConnectionData);
    connect(m_back, &QPushButton::clicked, this, &ClientTabView::back);

    this->setStyleSheet("QPushButton { height: 45px; }");

    m_connectionState->setFixedHeight(50);
    m_back->setFixedWidth(100);

    m_box->addWidget(m_host, 0, 0);
    m_box->addWidget(m_hostBox, 0, 1);
    m_box->addWidget(m_port, 1, 0);
    m_box->addWidget(m_portBox, 1, 1);
    m_box->addWidget(m_save, 2, 0);
    m_box->addWidget(m_back, 2, 1);
    m_box->addWidget(m_connectionState, 3, 0);

    setLayout(m_box);
}

void ClientTabView::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent)
        setFixedSize(wParent->width(), wParent->height());
}

void ClientTabView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
}

void ClientTabView::setIp(const QString &ip)
{
    m_hostBox->setText(ip);
}

void ClientTabView::setPort(const QString &port)
{
    m_portBox->setText(port);
}

void ClientTabView::saveConnectionData()
{
    emit startConnection(m_hostBox->text(), m_portBox->text().toInt());
}

void ClientTabView::setClientConnectionState(int error)
{
    qDebug() << error;
    if (error == 0) {
        m_connectionState->setText("Статус: Подлючен");
    } else {
        m_connectionState->setText("Статус: Ошибка подключения");
    }
}

void ClientTabView::back()
{
    emit showView(TestStartView);
}
