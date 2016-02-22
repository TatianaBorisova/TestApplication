#include "global.h"
#include "settingsview.h"
#include "testtabview.h"
#include "clienttabview.h"

#include <QGridLayout>
#include <QTabWidget>

SettingsView::SettingsView(QWidget *parent) :
    TestBaseView(parent),
    m_tab(new QTabWidget(this)),
    m_box(new QGridLayout(this)),
    m_testPage(new TestTabView(this)),
    m_clientSettingsPage(new ClientTabView(this))
{
    connect(m_testPage, &TestTabView::chosenTestName, this, &SettingsView::chosenTestName);

    m_tab->addTab(m_testPage, "Выбрать тест");
    m_tab->addTab(m_clientSettingsPage, "Подключение");

    m_box->addWidget(m_tab);
    setLayout(m_box);
}

void SettingsView::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent)
        setFixedSize(wParent->width(), wParent->height());

    m_testPage->setFixedSize(width()*0.9, height()*0.9);
}
