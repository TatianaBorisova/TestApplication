#include "startview.h"
#include "global.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QRect>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>

StartView::StartView(QWidget *parent)
    : TestBaseView(parent),
      m_hbox(new QHBoxLayout(this)),
      m_test(new QPushButton(this)),
      m_settings(new QPushButton(this))
{   
    connect(m_settings, &QPushButton::clicked, this, &StartView::openSettingsView);
    connect(m_test, &QPushButton::clicked, this, &StartView::openTestView);

    m_test->setIcon(QIcon(QPixmap(":res/test.png")));
    m_settings->setIcon(QIcon(QPixmap(":res/settings.png")));

    int btnSize = QApplication::desktop()->screenGeometry().width()*0.2;

    m_test->setIconSize(QSize(btnSize, btnSize));
    m_settings->setIconSize(QSize(btnSize, btnSize));
    m_hbox->setMargin(5);

    m_hbox->addWidget(m_test);
    m_hbox->addWidget(m_settings);

    setLayout(m_hbox);
}

void StartView::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent)
        setFixedSize(wParent->width(), wParent->height());

    int btnSize = width()*0.2;

    m_test->setIconSize(QSize(btnSize, btnSize));
    m_settings->setIconSize(QSize(btnSize, btnSize));

    m_hbox->setAlignment(m_test, Qt::AlignHCenter);
    m_hbox->setAlignment(m_settings, Qt::AlignHCenter);
}

void StartView::openTestView()
{
    emit showView(TestWayView);
}

void StartView::openSettingsView()
{
    emit showView(TestClientSettingsView);
}

StartView::~StartView()
{
}
