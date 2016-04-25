#include "testbaseview.h"

TestBaseView::TestBaseView(QWidget *parent) :
    QWidget(parent)
{
}

void TestBaseView::show()
{
    resize();
    QWidget::show();
}

void TestBaseView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resize();
}
