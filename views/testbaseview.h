#ifndef TESTBASEVIEW_H
#define TESTBASEVIEW_H

#include "global.h"

#include <QWidget>

class TestBaseView : public QWidget
{
    Q_OBJECT
public:
    explicit TestBaseView(QWidget *parent = 0);

signals:
    void showView(TestAppView view);

public slots:
    void show();

protected:
    virtual void resize()= 0;
};

#endif // TESTBASEVIEW_H
