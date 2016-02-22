#include "clienttabview.h"

#include <QGridLayout>

ClientTabView::ClientTabView(QWidget *parent) :
    TestBaseView(parent),
    m_box(new QGridLayout(this))
{
    setLayout(m_box);
}

void ClientTabView::resize()
{
}

void ClientTabView::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
}
