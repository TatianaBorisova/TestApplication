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
    m_save(new QPushButton("Подключиться", this)),
    m_back(new QPushButton("Назад", this)),
    m_connectionState(new QLabel("Статус: ", this)),
    m_box(new QGridLayout(this)),
    m_error(-1)
{
    connect(m_save, &QPushButton::clicked, this, &ClientTabView::saveConnectionData);
    connect(m_back, &QPushButton::clicked, this, &ClientTabView::back);

    this->setStyleSheet("QPushButton { height: 45px; }");

    m_connectionState->setFixedHeight(50);
    m_back->setFixedWidth(250);

    m_box->addWidget(m_host, 0, 0);
    m_box->addWidget(m_hostBox, 0, 1);
    m_box->addWidget(m_port, 1, 0);
    m_box->addWidget(m_portBox, 1, 1);
    m_box->addWidget(m_connectionState, 2, 0);

    m_box->addWidget(m_save, 3, 0);
    m_box->addWidget(m_back, 3, 1);

    m_box->setSpacing(50);
    m_box->setMargin(50);

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
    if (m_error == 0) {
        emit refuseConnection();
    } else {
        emit startConnection(m_hostBox->text(), m_portBox->text().toInt());
    }
}

void ClientTabView::setClientConnectionState(int error)
{
    m_error = error;
    if (m_error == 0) {
        m_connectionState->setText("Статус: Клиент подключен");
        m_save->setText("Отключиться");
    } else {
        m_connectionState->setText("Статус: Клиент отключен");
        m_save->setText("Подключиться");
    }
}

void ClientTabView::back()
{
    emit showView(TestStartView);
}
