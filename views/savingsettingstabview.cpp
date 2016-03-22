#include "savingsettingstabview.h"

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>

SavingSettingsTabView::SavingSettingsTabView(QWidget *parent) :
    TestBaseView(parent),
    m_savingPath(new QLabel(this)),
    m_resDbName(new QLineEdit(this)),
    m_choosePath(new QPushButton("Выбрать путь хранения", this)),
    m_grid(new QGridLayout(this))
{
    m_grid->addWidget(m_savingPath, 0, 0);
    m_grid->addWidget(m_resDbName,  0, 1);
    m_grid->addWidget(m_choosePath, 0, 2);
    setLayout(m_grid);
}

void SavingSettingsTabView::resize()
{
    QWidget *wParent = dynamic_cast<QWidget *>(parent());
    if (wParent)
        setFixedSize(wParent->width(), wParent->height());

}
