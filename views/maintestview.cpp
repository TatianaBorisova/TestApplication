#include "global.h"
#include "maintestview.h"
#include "testtabview.h"
#include "clienttabview.h"

#include <QGridLayout>
#include <QTabWidget>

MainTestView::MainTestView(QWidget *parent) :
    TestBaseView(parent),
    m_tab(new QTabWidget(this)),
    m_box(new QGridLayout(this)),
    m_testPage(new TestTabView(this))
{
    connect(m_testPage, &TestTabView::showView, this, &MainTestView::showView);
    connect(m_testPage, &TestTabView::chosenTestDB, this, &MainTestView::chosenTestDB);
    connect(m_testPage, &TestTabView::chosenTestName, this, &MainTestView::chosenTestName);
    connect(m_testPage, &TestTabView::tryGetTestsFromServer, this, &MainTestView::tryGetTestsFromServer);
    connect(this, &MainTestView::sendDownloadedFilePath, m_testPage, &TestTabView::addToChoiceBox);

    connect(this, &MainTestView::dbError, m_testPage, &TestTabView::dbError);
    connect(this, &MainTestView::readTests, m_testPage, &TestTabView::fillTestVariants);
    connect(this, &MainTestView::clientConnectionState, m_testPage, &TestTabView::setClientConnectionState);

    m_tab->addTab(m_testPage, "Выбрать тест");

    m_box->addWidget(m_tab);
    setLayout(m_box);
}

void MainTestView::resize()
{
    setFixedSize(width(), height());
    m_testPage->setFixedSize(width()*0.9, height()*0.9);
}
