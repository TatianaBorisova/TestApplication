#include "resultview.h"
#include "global.h"

#include <QLabel>
#include <QVBoxLayout>

static const int labelHeight = 60;

ResultView::ResultView(QWidget *parent) :
    QWidget(parent),
    m_vbox(new QVBoxLayout(this)),
    m_fio(new QLabel(this)),
    m_group(new QLabel(this)),
    m_header(new QLabel(this)),
    m_sore(new QLabel(this))
{
    setFixedSize(500, 400);

    QFont font("Times", 16);

    m_header->setFont(font);
    m_fio->setFont(font);
    m_group->setFont(font);
    m_sore->setFont(font);

    m_header->setAlignment(Qt::AlignHCenter);
    m_fio->setAlignment(Qt::AlignHCenter);
    m_group->setAlignment(Qt::AlignHCenter);
    m_sore->setAlignment(Qt::AlignHCenter);

    m_fio->setWordWrap(true);
    m_fio->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    m_group->setWordWrap(true);

//    m_header->setFixedSize(width(), labelHeight);
//    m_fio->setFixedSize(width(), labelHeight);
//    m_group->setFixedSize(width(), labelHeight);
//    m_sore->setFixedSize(width(), labelHeight);

    m_header->setText("Ваш результат:");

    m_vbox->setSizeConstraint(QLayout::SetMinimumSize);
    m_vbox->setAlignment(Qt::AlignLeft);
    m_vbox->addWidget(m_fio);
    m_vbox->addWidget(m_group);
    m_vbox->addWidget(m_header);
    m_vbox->addWidget(m_sore);

    setLayout(m_vbox);
}

void ResultView::finishTestResult(const StudentResult &result)
{
    m_fio->setText(result.firstName + " " + result.secondName + " " + result.surname);
    m_group->setText(result.group);
    m_sore->setText("Результирующий бал: " + QString::number(result.score) + " из " + QString::number(result.answerInfo.count()*2) + " возможных.");
}
