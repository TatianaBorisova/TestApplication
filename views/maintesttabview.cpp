#include "global.h"
#include "maintesttabview.h"
#include "testtabview.h"
#include "clienttabview.h"

#include <QGridLayout>
#include <QTabWidget>

MainTestTabView::MainTestTabView(QWidget *parent) :
    TestBaseView(parent),
    m_tab(new QTabWidget(this)),
    m_box(new QGridLayout(this)),
    m_testPage(new TestTabView(this))
{
    connect(m_testPage, &TestTabView::showView, this, &MainTestTabView::showView);
    connect(m_testPage, &TestTabView::chosenTestDB, this, &MainTestTabView::chosenTestDB);
    connect(m_testPage, &TestTabView::chosenTestName, this, &MainTestTabView::chosenTestName);
    connect(m_testPage, &TestTabView::tryGetTestsFromServer, this, &MainTestTabView::tryGetTestsFromServer);
    connect(this, &MainTestTabView::sendDownloadedFilePath, m_testPage, &TestTabView::addToChoiceBox);

    connect(this, &MainTestTabView::dbError, m_testPage, &TestTabView::dbError);
    connect(this, &MainTestTabView::readTests, m_testPage, &TestTabView::fillTestVariants);
    connect(this, &MainTestTabView::clientConnectionState, m_testPage, &TestTabView::setClientConnectionState);

    m_tab->addTab(m_testPage, "Выбрать тест");

    m_box->addWidget(m_tab);
    setLayout(m_box);
}

void MainTestTabView::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent)
        setFixedSize(wParent->width(), wParent->height());

    m_testPage->setFixedSize(width()*0.9, height()*0.9);
}
