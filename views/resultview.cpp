#include "resultview.h"
#include "global.h"

#include <QLabel>
#include <QVBoxLayout>

static const int labelHeight = 60;

ResultView::ResultView(QWidget *parent) :
    TestBaseView(parent),
    m_vbox(new QVBoxLayout(this)),
    m_fio(new QLabel(this)),
    m_group(new QLabel(this)),
    m_header(new QLabel(this)),
    m_sore(new QLabel(this))
{
    this->setStyleSheet("font-family: Arial; font-style: normal; font-size: 20pt;");
    m_header->setAlignment(Qt::AlignHCenter);
    m_fio->setAlignment(Qt::AlignHCenter);
    m_group->setAlignment(Qt::AlignHCenter);
    m_sore->setAlignment(Qt::AlignHCenter);

    m_fio->setWordWrap(true);
    m_fio->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    m_group->setWordWrap(true);

    m_header->setText("Ваш результат:");

    m_vbox->setContentsMargins(80, 150, 80, 80);
    m_vbox->addWidget(m_fio);
    m_vbox->addWidget(m_group);
    m_vbox->addWidget(m_header);
    m_vbox->addWidget(m_sore);

    setLayout(m_vbox);
}

void ResultView::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent)
        setFixedSize(wParent->width(), wParent->height());
}

void ResultView::finishTestResult(const StudentResult &result)
{
    m_fio->setText(result.firstName + " " + result.secondName + " " + result.surname);
    m_group->setText(result.group);
    m_sore->setText(QString::number(result.score) + " из " + QString::number(result.answerInfo.count()*2) + " возможных.");
}
