#include "settingsview.h"
#include "clienttabview.h"
#include "savingsettingstabview.h"

#include <QTabWidget>
#include <QVBoxLayout>

SettingsView::SettingsView(QWidget *parent) :
    TestBaseView(parent),
    m_tab(new QTabWidget(this)),
    m_netwotkView(new ClientTabView(this)),
    m_saveView(new SavingSettingsTabView(this))
{
    connect(m_netwotkView, &ClientTabView::showView, this, &SettingsView::showView);
    connect(m_saveView, &SavingSettingsTabView::showView, this, &SettingsView::showView);
    connect(this, &SettingsView::finishTestResult, m_saveView, &SavingSettingsTabView::saveTestResultInDB);

    connect(m_netwotkView, &ClientTabView::refuseConnection, this, &SettingsView::refuseConnection);
    connect(m_netwotkView, &ClientTabView::startConnection, this, &SettingsView::startConnection);

    QVBoxLayout *vbox = new QVBoxLayout();
    m_tab->addTab(m_netwotkView, "Сетевое подключение");
    m_tab->addTab(m_saveView, "Настройки сохранения");

    vbox->addWidget(m_tab);
    setLayout(vbox);
}

void SettingsView::resize()
{
    m_tab->setFixedSize(width()*0.98, height()*0.95);
    m_saveView->setFixedSize(m_tab->width(), m_tab->height());
    m_netwotkView->setFixedSize(m_tab->width(), m_tab->height());
}

void SettingsView::setClientConnectionState(int error)
{
    m_netwotkView->setClientConnectionState(error);
}

void SettingsView::setIp(const QString &ip)
{
    m_netwotkView->setIp(ip);
}

void SettingsView::setPort(int port)
{
    m_netwotkView->setPort(port);
}
