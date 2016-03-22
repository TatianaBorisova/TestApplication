#include "settingstabview.h"
#include "clienttabview.h"

#include <QTabWidget>
#include <QVBoxLayout>

SettingsTabView::SettingsTabView(QWidget *parent) :
    TestBaseView(parent),
    m_tab(new QTabWidget(this)),
    m_netwotkView(new ClientTabView(this))
{
    connect(m_netwotkView, &ClientTabView::showView, this, &SettingsTabView::showView);

    QVBoxLayout *vbox = new QVBoxLayout();
    m_tab->addTab(m_netwotkView, "Сетевое подключение");

    vbox->addWidget(m_tab);

    setLayout(vbox);
}

void SettingsTabView::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent)
        setFixedSize(wParent->width(), wParent->height());

    m_tab->setFixedSize(width()*0.98, height()*0.95);
}

void SettingsTabView::setClientConnectionState(int error)
{
    m_netwotkView->setClientConnectionState(error);
}

void SettingsTabView::setIp(const QString &ip)
{
    m_netwotkView->setIp(ip);
}

void SettingsTabView::setPort(int port)
{
    m_netwotkView->setPort(port);
}
