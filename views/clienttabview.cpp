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
    m_connectToChosen(new QPushButton("Подключиться к выбранному IP", this)),
    m_connectToAny(new QPushButton("Поиск IP подключения", this)),
    m_back(new QPushButton("Вернуться на главную", this)),
    m_connectionState(new QLabel("Статус: ", this)),
    m_box(new QGridLayout(this)),
    m_error(-1)
{
    connect(m_connectToChosen, &QPushButton::clicked, this, &ClientTabView::sendChosenConnectionData);
    connect(m_connectToAny, &QPushButton::clicked, this, &ClientTabView::setDefaultConnectionData);
    connect(m_back, &QPushButton::clicked, this, &ClientTabView::back);

    this->setStyleSheet("QPushButton { height: 45px; }"
                        "QLineEdit { height: 50px; }");

    m_connectionState->setFixedHeight(50);
    m_back->setFixedWidth(250);

    m_box->addWidget(m_host, 0, 0);
    m_box->addWidget(m_hostBox, 0, 1);
    m_box->addWidget(m_connectToChosen, 0, 2);
    m_box->addWidget(m_connectToAny, 0, 3);
    m_box->addWidget(m_port, 1, 0);
    m_box->addWidget(m_portBox, 1, 1);
    m_box->addWidget(m_connectionState, 2, 0);

    m_box->addWidget(m_back, 3, 0);

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

void ClientTabView::setPort(int port)
{
    m_portBox->setText(QString::number(port));
}

void ClientTabView::sendChosenConnectionData()
{
    //Check later!
    m_connectionState->setText("Статус: Подключение...");
    m_hostBox->setDisabled(true);
    m_portBox->setDisabled(true);

    if (m_error == 0) {
        emit refuseConnection();
    } else {
        emit startConnection(m_hostBox->text(), m_portBox->text().toInt());
    }
}

void ClientTabView::setDefaultConnectionData()
{
    //Check later!
    m_connectionState->setText("Статус: Подключение...");
    m_hostBox->setDisabled(true);
    m_portBox->setDisabled(true);

    if (m_error == 0) {
        emit refuseConnection();
    } else {
        emit startConnection(zeroHost, m_portBox->text().toInt());
    }
}

void ClientTabView::setClientConnectionState(int error)
{
    m_error = error;
    if (m_error == 0) {
        m_connectionState->setText("Статус: Клиент подключен");
        m_connectToChosen->setText("Отключиться");
        m_connectToAny->setDisabled(true);
        m_hostBox->setDisabled(true);
        m_portBox->setDisabled(true);
    } else {
        m_connectToAny->setDisabled(false);
        m_connectionState->setText("Статус: Клиент отключен");
        m_connectToChosen->setText("Подключиться к выбранному IP");
        m_connectToAny->setText("Поиск IP подключения");
        m_hostBox->setDisabled(false);
        m_portBox->setDisabled(false);
    }
}

void ClientTabView::back()
{
    emit showView(TestStartView);
}
